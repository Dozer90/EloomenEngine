#pragma once

#include "utility/defines.h"

#include <EASTL/functional.h>
#include <EASTL/unordered_map.h>

namespace sge {
    template <typename... Args>
    class observer {
    public:
        SGE_DECLARE_ID_T;

    public:
        using callback_t = eastl::function<void(Args...)>;

        id_t subscribe(callback_t fn) {
            const id_t id = mNextID++;
            mCallbacks[id] = fn;
            return id;
        }

        id_t subscribe(void (*fn)(Args...)) {
            return subscribe(callback_t(fn));
        }


        template <typename T>
        id_t subscribe(void (T::*fn)(Args...), T* obj) {
            return subscribe(callback_t([obj, fn](Args... args) { (obj->*fn)(args...); }));
        }

        void unsubscribe(id_t id) {
            mCallbacks.erase(static_cast<size_t>(id));
        }

        void unsubscribe_all() {
            mCallbacks.clear();
        }

        void broadcast(Args... args) const {
            for (auto& [id, callback] : mCallbacks) {
                callback(args...);
            }
        }

    private:
        size_t mNextID = 0;
        eastl::unordered_map<size_t, callback_t> mCallbacks;
    };
}