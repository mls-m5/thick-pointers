#pragma once

#include "trait.h"

// This is a reference implementation to base macros on
struct RawMovable {
    struct FunctionTable {
        void (tpimpl::FunctionMemberDummy::*move)(int, int);
        void (tpimpl::FunctionMemberDummy::*jump)(bool);
    };

    tpimpl::FunctionMemberDummy *p = nullptr;
    FunctionTable *_ftable = nullptr;

    template <typename T>
    RawMovable(T *p) {
        _ftable = tpimpl::functionTableInstance<
            T,
            RawMovable,
            FunctionTable,
            tpimpl::FunctionTypeStruct<decltype(FunctionTable::move),
                                       decltype(&T::move),
                                       &T::move>,
            tpimpl::FunctionTypeStruct<decltype(FunctionTable::jump),
                                       decltype(&T::jump),
                                       &T::jump>>();
        this->p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(p);
    }

    void move(int x, int y) {
        return (p->*_ftable->move)(x, y);
    }

    // Alternative... trashes ide-help, but works
    template <typename... Args>
    void jump(Args... args) {
        return (p->*_ftable->jump)(args...);
    }
};
