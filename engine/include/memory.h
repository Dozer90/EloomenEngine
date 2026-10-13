#pragma once

#include "utility/defines.h"

#include <limits>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <atomic>
#include <type_traits>
#include <utility>
#include <span>

namespace sge {

    namespace detail {
        inline std::atomic<uint32_t> gNextOwnerId{ 1 };

        inline uint32_t allocate_storage_owner() {
            auto candidate = gNextOwnerId.load(std::memory_order_relaxed);
            for (;;) {
                if (candidate == std::numeric_limits<uint32_t>::max()) {
                    throw std::length_error("No available storage owner IDs");
                }
                if (gNextOwnerId.compare_exchange_weak(candidate, candidate + 1,
                    std::memory_order_relaxed)) {
                    return candidate;
                }
            }
        }
    }

    template <typename T>
    class dense_storage {

        static_assert(!std::is_same_v<T, bool>,
            "dense_storage<bool> is unsupported: vector<bool> uses packed bits");

    public:
        struct handle_t {
        public:
            constexpr handle_t() = default; // Invalid handle

            constexpr bool operator==(const handle_t &) const = default;

        private:
            friend class sge::dense_storage;

            constexpr handle_t(uint32_t slot, uint32_t generation, uint32_t owner)
                : mSlot(slot), mGeneration(generation), mOwner(owner) {}

            uint32_t mSlot = std::numeric_limits<uint32_t>::max();
            uint32_t mGeneration = 0;
            uint32_t mOwner = 0;
        };

        static constexpr handle_t INVALID_HANDLE{};

    public:
        dense_storage() = default;

        dense_storage(const dense_storage &) = delete;
        dense_storage &operator=(const dense_storage &) = delete;

        dense_storage(dense_storage &&) = delete;
        dense_storage &operator=(dense_storage &&) = delete;


    public:

        //$ Create a new value in the storage and return a handle to it.
        //$ arg0: ${T} => The value to store.
        //$ return: handle_t => A handle to the newly created value.
        handle_t create(const T &value) {
            if (mFreeSlot != INVALID_INDEX) {
                const auto slotIndex = mFreeSlot;
                auto &slot = mSlots[slotIndex];

                const auto nextFree = slot.denseIndex;
                const auto denseIndex =
                    static_cast<uint32_t>(mValues.size());

                mDenseToSlot.push_back(slotIndex);

                try {
                    mValues.push_back(value);
                } catch (...) {
                    mDenseToSlot.pop_back();
                    throw;
                }

                // Commit only after both appends succeed.
                mFreeSlot = nextFree;
                slot.denseIndex = denseIndex;

                return handle_t{ slotIndex, slot.generation, mOwner };
            }

            if (mSlots.size() >= INVALID_INDEX) {
                throw std::length_error("Storage has no available slots");
            }

            const auto slot = static_cast<uint32_t>(mSlots.size());
            const auto denseIndex = static_cast<uint32_t>(mValues.size());

            mSlots.push_back({ denseIndex, 1 });

            try {
                mDenseToSlot.push_back(slot);
                mValues.push_back(value);
            } catch (...) {
                if (mDenseToSlot.size() > denseIndex) {
                    mDenseToSlot.pop_back();
                }
                mSlots.pop_back();
                throw;
            }

            return handle_t{ slot, 1, mOwner };
        }
     
        //$ Get the number of live values in the storage.
        //$ return: size_t => The number of live values.
        size_t size() const noexcept {
            return mValues.size();
        }

        //$ Remove a value from the storage by its handle.
        //$ arg0: handle_t => The handle of the value to remove.
        //$ return: bool => True if the value was removed, false if the handle was invalid.
        bool remove(const handle_t &handle) noexcept {
            static_assert(std::is_nothrow_move_assignable_v<T>,
                "Compact deletion requires non-throwing move assignment");
            static_assert(std::is_nothrow_destructible_v<T>,
                "Stored values must have non-throwing destructors");

            if (!is_valid(handle)) {
                return false;
            }

            auto &slot = mSlots[handle.mSlot];
            const auto removedIndex = slot.denseIndex;
            const auto lastIndex = mValues.size() - 1;

            if (removedIndex != lastIndex) {
                mValues[removedIndex] = std::move(mValues.back());

                const auto movedSlot = mDenseToSlot.back();
                mDenseToSlot[removedIndex] = movedSlot;
                mSlots[movedSlot].denseIndex = removedIndex;
            }

            mValues.pop_back();
            mDenseToSlot.pop_back();

            slot.denseIndex = INVALID_INDEX;

            if (slot.generation < std::numeric_limits<uint32_t>::max()) {
                ++slot.generation;

                slot.denseIndex = mFreeSlot;
                mFreeSlot = handle.mSlot;
            }

            return true;
        }

        //$ Check if a handle is valid and points to a live value in the storage.
        //$ arg0: handle_t => The handle to check.
        //$ return: bool => True if the handle is valid, false otherwise.
        bool is_valid(const handle_t &handle) const SGE_NOEXCEPT {
            if (handle.mOwner != mOwner) {
                return false;
            }
            if (handle.mSlot >= mSlots.size()) {
                return false;
            }
            const auto &slot = mSlots[handle.mSlot];
            return slot.generation == handle.mGeneration
                && slot.denseIndex < mValues.size()
                && mDenseToSlot[slot.denseIndex] == handle.mSlot;
        }

        //$ Get a pointer to the value associated with a handle, or nullptr if the handle is invalid.
        //$ arg0: handle_t => The handle of the value to retrieve.
        //$ return: ${T}* => A pointer to the value, or nullptr if the handle is invalid.
        T *try_get(const handle_t &handle) SGE_NOEXCEPT {
            if (!is_valid(handle)) {
                return nullptr;
            }
            const auto &slot = mSlots[handle.mSlot];
            return &mValues[slot.denseIndex];
        }

        //$ Get a const pointer to the value associated with a handle, or nullptr if the handle is invalid.
        //$ arg0: handle_t => The handle of the value to retrieve.
        //$ return: const ${T}* => A const pointer to the value, or nullptr if the handle is invalid.
        const T *try_get(const handle_t &handle) const SGE_NOEXCEPT {
            if (!is_valid(handle)) {
                return nullptr;
            }
            const auto &slot = mSlots[handle.mSlot];
            return &mValues[slot.denseIndex];
        }

        //$ Get a span of all live values in the storage.
        //$ return: std::span<${T}> => A span of all live values.
        //$ important: Borrowed view! Do not retain across create/remove operations. \
        //$            Do not reorder elements; that would break the handle mapping.
        std::span<T> values() SGE_NOEXCEPT {
            return { mValues.data(), mValues.size() };
        }

        //$ Get a const span of all live values in the storage.
        //$ return: std::span<const ${T}> => A const span of all live values.
        std::span<const T> values() const SGE_NOEXCEPT {
            return { mValues.data(), mValues.size() };
        }

    private:
        struct slot_record {
            uint32_t denseIndex;
            uint32_t generation;
        };

        static constexpr uint32_t INVALID_INDEX =
            std::numeric_limits<uint32_t>::max();

        const uint32_t mOwner = detail::allocate_storage_owner();

        std::vector<T> mValues;                 // Live values, packed densely
        std::vector<slot_record> mSlots;        // Slots -> Current dense index and generation
        std::vector<uint32_t> mDenseToSlot;     // Dense index -> Owning slot index

        uint32_t mFreeSlot = INVALID_INDEX;
    };
}