#pragma once

#include "thickpointer.h"

// This is a reference implementation to base macros on
struct RawMovable {
    struct FunctionTable {
        void (FunctionMemberDummy::*move)(int, int);
        void (FunctionMemberDummy::*jump)(bool);
    };

    FunctionMemberDummy *p = nullptr;
    FunctionTable *_ftable = nullptr;

    template <typename T>
    RawMovable(T *p) {
        _ftable = functionTableInstance<
            T,
            RawMovable,
            FunctionTable,
            FunctionTypeStruct<decltype(FunctionTable::move),
                               decltype(&T::move),
                               &T::move>,
            FunctionTypeStruct<decltype(FunctionTable::jump),
                               decltype(&T::jump),
                               &T::jump>>();
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }

    void move(int x, int y) {
        return (p->*_ftable->move)(x, y);
    }

    // Alternative, but trashes ide-help, but works
    template <typename... Args>
    void jump(Args... args) {
        return (p->*_ftable->jump)(args...);
    }
};
