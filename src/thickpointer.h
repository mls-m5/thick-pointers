#pragma once
#include <tuple>

struct FunctionMemberDummy {
    // The only reason for this struct is because c++ refuses to cast pointers
    // to member functions to other types
};

template <typename... F>
using FunctionTable = std::tuple<F...>;

template <typename Trait, typename Type, typename FT, typename OFT, OFT FP>
FunctionTable<FT> *functionTableInstance() {
    static FunctionTable<FT> instance =
        FunctionTable<FT>{reinterpret_cast<FT>(FP)};
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

template <typename... F>
struct TraitImpl : virtual ThickPointerBase {
    FunctionTable<F...> *_ftable;

    template <typename T, typename Trait, typename FT, typename OFT, OFT FP>
    void init(T *p) {
        _ftable = functionTableInstance<Trait, T, FT, OFT, FP>();
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }
};

// TODO: Hide this in some ugly macro... :'(
struct Movable : public virtual ThickPointerBase {
    using moveFT = void (FunctionMemberDummy::*)(int, int);
    TraitImpl<moveFT> _impl;

    template <typename T>
    Movable(T *p) {
        _impl.init<T, Movable, moveFT, decltype(&T::move), &T::move>(p);
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }

    void move(int x, int y) {
        auto f = std::get<0>(*_impl._ftable);
        (p->*f)(x, y);
    }
};
