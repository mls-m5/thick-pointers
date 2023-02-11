#pragma once

#include "trait.h"

// This is a reference implementation to base macros on
class RawMovable {
    struct FunctionTable {
        void (*destructor)(void *ptr);
        void (tpimpl::FunctionMemberDummy::*move)(int, int);
        float (tpimpl::FunctionMemberDummy::*jump)(bool);
    };

    tpimpl::FunctionMemberDummy *_p = nullptr;
    const FunctionTable *_ftable = nullptr;

public:
    template <typename T>
    RawMovable(T *p) {
        auto destructor = [](T *ptr) { delete ptr; };

        _ftable = tpimpl::FunctionTableInstance<
            T,
            RawMovable,
            FunctionTable,
            tpimpl::FunctionDestructorStruct<T>,
            tpimpl::FunctionTypeStruct<decltype(FunctionTable::move),
                                       decltype(&T::move),
                                       &T::move>,
            tpimpl::FunctionTypeStruct<decltype(FunctionTable::jump),
                                       decltype(&T::jump),
                                       &T::jump>>::table;
        this->_p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(_p);
    }

    RawMovable() = default;

    auto move(int x, int y) {
        return (_p->*_ftable->move)(x, y);
    }

    // Alternative... trashes ide-help, but works
    template <typename... Args>
    auto jump(Args &&...args) {
        return (_p->*_ftable->jump)(std::forward<Args>(args)...);
    }
};
