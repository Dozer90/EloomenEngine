#pragma once

#include "utility/defines.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_set.h>
#include <EASTL/type_traits.h>

#include <memory>


namespace eloo {
    template <typename T, int InitialSize, float ExpansionScalar = 0.5f>
    class managed_memory_block {
    public:
        managed_memory_block() {
            mData.resize(InitialSize);
            if constexpr (eastl::is_trivially_constructible_v<T>) {
                memset(mData.data(), 0, InitialSize * sizeof(T));
            } else {
                std::uninitialized_fill(mData.begin(), mData.end(), T());
            }
        }

        template <typename... Args>
        size_t emplace(Args&&... args) {
            size_t id = mSize;
            if (!mUnusedIDs.empty()) {
                id = *mUnusedIDs.begin();
                mUnusedIDs.erase(mUnusedIDs.begin());
            } else if (++mSize >= mData.size()) {
                expand();
            }
            if constexpr (!eastl::is_trivially_destructible_v<T>) {
                mData[id].~T();
            }
            new (&mData[id]) T(std::forward<Args>(args)...);
            return id;
        }

        size_t push(T val) {
            return push(std::move(val));
        }

        size_t push(T&& val) {
            size_t id = mSize;
            if (!mUnusedIDs.empty()) {
                id = *mUnusedIDs.begin();
                mUnusedIDs.erase(mUnusedIDs.begin());
            } else if (++mSize >= mData.size()) {
                expand();
            }
            mData[id] = std::move(val);
            return id;
        }

        size_t push(const T* src, size_t count) {
            while (mSize + count > mData.size()) {
                expand();
            }
            memccpy(mData.data() + mSize, src, 0, count * sizeof(T));
            mSize += count;
            return mSize - count; // Return the starting index of the copied elements
        }

        bool remove(size_t id) {
            if (!is_valid(id)) {
                ELOO_ASSERT_FALSE("Cannot remove invalid id %i from managed_memory_block.", id);
                return false;
            }
            if (!mUnusedIDs.insert(id).second) {
                ELOO_ASSERT_FALSE("ID %i is already in managed_memory_block's pool. This should not happen.", id);
                return false;
            }
            mData[id] = T();
            return true;
        }

        bool is_valid(size_t id) const {
            return id < mSize && mUnusedIDs.find(id) == mUnusedIDs.end();
        }

        bool try_get(size_t id, T& out) const {
            if (is_valid(id)) {
                out = mData[id];
                return true;
            }
            return false;
        }

        T& get(size_t index) {
            ELOO_ASSERT(index < mSize, "Index out of range");
            return mData[index];
        }

        const T& get(size_t index) const {
            ELOO_ASSERT(index < mSize, "Index out of range");
            return mData[index];
        }

        T& operator[](size_t index) {
            ELOO_ASSERT(index < mSize, "Index out of range");
            return get(index);
        }

        const T& operator[](size_t index) const {
            ELOO_ASSERT(index < mSize, "Index out of range");
            return get(index);
        }

        void set(size_t index, T val) {
            ELOO_ASSERT(index < mSize, "Index out of range");
            mData[index] = val;
        }

        size_t count() const {
            return mSize;
        }

    private:
        void expand() {
            const size_t oldSize = mData.size();
            const size_t sizeExpansion = static_cast<size_t>(oldSize * ExpansionScalar + 0.5f);
            mData.resize(oldSize + sizeExpansion);
            if constexpr (eastl::is_trivially_constructible_v<T>) {
                memset(mData.data() + oldSize, 0, sizeExpansion * sizeof(T));
            } else {
                std::uninitialized_fill(mData.begin() + oldSize, mData.end(), T());
            }
        }

    private:
        uint32_t mSize = 0;
        eastl::vector<T> mData;
        eastl::unordered_set<size_t> mUnusedIDs;
    };

    template <typename T, int ElementCount, int InitialSize, float ExpansionScalar = 0.5f>
    class managed_sequential_memory_block {
        static_assert(ElementCount > 1, "ElementCount must be greater than 1. Use 'managed_memory_block' if there is only one element per ID");

    public:
        managed_sequential_memory_block() {
            mData.resize(InitialSize * ElementCount);
            if constexpr (eastl::is_trivially_constructible_v<T>) {
                memset(mData.data(), 0, InitialSize * ElementCount * sizeof(T));
            } else {
                std::uninitialized_fill(mData.begin(), mData.end(), T());
            }
        }

        size_t push(std::initializer_list<T> values) {
            ELOO_ASSERT_FATAL(values.size() == ElementCount, "Invalid number of values provided to push");

            size_t id = mSize;
            if (!mUnusedIDs.empty()) {
                id = *mUnusedIDs.begin();
                mUnusedIDs.erase(mUnusedIDs.begin());
            } else if (mData.size() <= (++mSize) * ElementCount) {
                expand();
            }

            eastl::copy(values.begin(), values.end(), mData.data() + id * ElementCount);

            return id;
        }

        bool try_remove(size_t id) {
            const bool successful = is_valid(id) && mUnusedIDs.insert(id).second;
            if (successful) {
                if constexpr (eastl::is_trivially_constructible_v<T>) {
                    memset(mData.data() + id * ElementCount, 0, ElementCount * sizeof(T));
                } else {
                    std::uninitialized_fill(mData.begin() + id * ElementCount, mData.begin() + (id + 1) * ElementCount, T());
                }
            }
            return successful;
        }

        bool is_valid(size_t id) const {
            return id < mSize && mUnusedIDs.find(id) == mUnusedIDs.end();
        }

        bool try_get(size_t id, size_t elementOffset, T& out) const {
            if (is_valid(id)) {
                out = mData[id * ElementCount + elementOffset];
                return true;
            }
            return false;
        }

        T& get(size_t id, size_t elementOffset) {
            ELOO_ASSERT(is_valid(id), "ID is not valid");
            return mData[id * ElementCount + elementOffset];
        }

        const T& get(size_t id, size_t elementOffset) const {
            ELOO_ASSERT(is_valid(id), "ID is not valid");
            return mData[id * ElementCount + elementOffset];
        }

        T* operator[](size_t id) {
            ELOO_ASSERT(is_valid(id), "ID is not valid");
            return &get(id, 0);
        }

        const T* operator[](size_t id) const {
            ELOO_ASSERT(is_valid(id), "ID is not valid");
            return &get(id, 0);
        }

        void set(size_t id, size_t elementOffset, T val) {
            ELOO_ASSERT(is_valid(id), "ID is not valid");
            mData[id * ElementCount + elementOffset] = val;
        }

        size_t count() const {
            return mSize;
        }

    private:
        void expand() {
            const size_t oldSize = mSize;
            const size_t sizeExpansion = static_cast<size_t>(oldSize * ExpansionScalar + 0.5f);
            mData.resize((oldSize + sizeExpansion) * ElementCount);
            if constexpr (eastl::is_trivially_constructible_v<T>) {
                memset(mData.data() + oldSize * ElementCount, 0, sizeExpansion * sizeof(T) * ElementCount);
            } else {
                std::uninitialized_fill(mData.begin() + oldSize * ElementCount, mData.end(), T());
            }
        }


    private:
        uint32_t mSize = 0;
        eastl::vector<T> mData;
        eastl::unordered_set<size_t> mUnusedIDs;
    };
}