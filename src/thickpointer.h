#pragma once
#include <tuple>

struct FunctionMemberDummy {
    // The only reason for this struct is because c++ refuses to cast pointers
    // to member functions to other types
};

template <typename... F>
using FunctionTable = std::tuple<F...>;

template <typename Trait, typename Type, typename F>
FunctionTable<F> *functionTableInstance() {
    static FunctionTable<F> instance{reinterpret_cast<F>(&Type::move)};
    return &instance;
}

struct ThickPointerBase {
    FunctionMemberDummy *p;
};

template <typename Trait>
struct ThickPointer : public Trait {
    template <typename Type>
    ThickPointer(Type *p)
        : Trait{p} {}

    template <typename Type>
    ThickPointer &operator=(Type *p) {
        Trait::operator=(p);
        return *this;
    }
};

// TODO: Hide this in some ugly macro... :'(
struct Movable : virtual ThickPointerBase {
    using ft = void (FunctionMemberDummy::*)(int, int);
    FunctionTable<ft> *_ftable;

    template <typename T>
    Movable(T *p)
        : _ftable{functionTableInstance<Movable, T, ft>()} {
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }

    template <typename T>
    Movable &operator=(T *p) {
        _ftable = functionTableInstance<Movable, T, ft>();
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
        return *this;
    }

    void move(int x, int y) {
        auto f = std::get<0>(*_ftable);
        (p->*f)(x, y);
    }
};
