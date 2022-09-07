#pragma once
#include <tuple>

struct FunctionMemberDummy {
    // The only reason for this struct is because c++ refuses to cast pointers
    // to member functions to other types
};

// Used to send template parameters in larger packs
template <typename DummyType, typename FunctionType, FunctionType FunctionValue>
struct FunctionTypeStruct {
    using FT = DummyType;     // Used to store pointer
    using OFT = FunctionType; // The actual pointer type
    constexpr static inline FunctionType FP = FunctionValue;
};

// The actual pointer type
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

/// Create a single trait function table per class
template <typename Type, typename Trait, typename Table, typename... FuncS>
auto *functionTableInstance() {
    // I would prefere to not create this on the heap, but the compiler throws
    // mysterious type conversion error when not so I give up
    static auto table =
        new Table{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};

    return table;
}

template <typename... F>
using FunctionTable = std::tuple<F...>;

// TODO: Hide this in some ugly macro... :'(
struct Movable {
    struct FunctionTable {
        void (FunctionMemberDummy::*move)(int, int);
        void (FunctionMemberDummy::*jump)(bool);
    };

    FunctionMemberDummy *p = nullptr;
    FunctionTable *_ftable;

    template <typename T>
    Movable(T *p) {
        _ftable = functionTableInstance<
            T,
            Movable,
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

    // Alternative, but trashes ide-help
    template <typename... Args>
    void jump(Args... args) {
        return (p->*_ftable->jump)(args...);
    }
};
