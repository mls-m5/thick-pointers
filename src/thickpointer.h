#pragma once
#include <tuple>

struct FunctionMemberDummy {
    // The only reason for this struct is because c++ refuses to cast pointers
    // to member functions to other types
};

template <typename... F>
using FunctionTable = std::tuple<F...>;

// Used to send template parameters in larger packs
template <typename DummyType, typename FunctionType, FunctionType FunctionValue>
struct FunctionTypeStruct {
    using FT = DummyType;     // Used to store pointer
    using OFT = FunctionType; // The actual pointer type
    constexpr static inline FunctionType FP = FunctionValue;
};

template <typename Trait, typename Type, typename... FuncS>
FunctionTable<typename FuncS::FT...> *functionTableInstance() {
    static FunctionTable<typename FuncS::FT...> instance =
        FunctionTable<typename FuncS::FT...>{
            (reinterpret_cast<typename FuncS::FT>(FuncS::FP), ...)};
    return &instance;
}

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
struct TraitImpl {
    FunctionTable<F...> *_ftable;

    template <typename T, typename Trait, typename... FuncS>
    void init(T *p) {
        _ftable = functionTableInstance<Trait, T, FuncS...>();
    }
};

// TODO: Hide this in some ugly macro... :'(
struct Movable {
    using moveFT = void (FunctionMemberDummy::*)(int, int);

    TraitImpl<moveFT> _impl;
    FunctionMemberDummy *p = nullptr;

    template <typename T>
    Movable(T *p) {
        _impl.init<T,
                   Movable,
                   FunctionTypeStruct<moveFT, decltype(&T::move), &T::move>>(p);
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }

    void move(int x, int y) {
        auto f = std::get<0>(*_impl._ftable);
        (p->*f)(x, y);
    }
};
