#pragma once

#include "trait.h"

// This is a reference implementation to base macros on
class RawMovable {
protected:
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
        this->_p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(p);
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

class RawUniqueMovable : public RawMovable {
public:
    template <typename T>
    RawUniqueMovable(T *p)
        : RawMovable{p} {}

    RawUniqueMovable(RawUniqueMovable &&other) {
        _p = other._p;
        _ftable = other._ftable;
        other._p = nullptr;
        other._ftable = nullptr;
    }

    RawUniqueMovable(const RawUniqueMovable &other) = delete;

    RawUniqueMovable &operator=(RawUniqueMovable &&other) {
        _p = other._p;
        other._p = nullptr;
        return *this;
    }

    RawUniqueMovable &operator=(const RawUniqueMovable &other) = delete;

    ~RawUniqueMovable() {
        if (_p) {
            _ftable->destructor(_p);
        }
    }
};

template <typename T, typename... Args>
RawUniqueMovable makeUniqueMovable(Args... args) {
    return RawUniqueMovable{new T{args...}};
}
