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

#define TRAIT_FTABLE_FUNCTION(name, ret, args)                                 \
    ret(FunctionMemberDummy::*name) args;

#define TRAIT_FTABLE(name, f1)                                                 \
    struct FunctionTable {                                                     \
        TRAIT_FTABLE_FUNCTION f1                                               \
    };

#define CONSTRUCTOR_FUNCTION(name, ret, args)                                  \
    FunctionTypeStruct<decltype(FunctionTable::name),                          \
                       decltype(&T::name),                                     \
                       &T::name>

#define FUNCTION_DEFINITON(name, ret, args)                                    \
    template <typename... Args>                                                \
    void name(Args... a) {                                                     \
        return (p->*_ftable->name)(a...);                                      \
    }

#define TRAIT_BODY(name, f1)                                                   \
    FunctionMemberDummy *p = nullptr;                                          \
    FunctionTable *_ftable = nullptr;                                          \
    template <typename T>                                                      \
    name(T *p) {                                                               \
        _ftable = functionTableInstance<T,                                     \
                                        name,                                  \
                                        FunctionTable,                         \
                                        CONSTRUCTOR_FUNCTION f1>();            \
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);                  \
    }                                                                          \
    FUNCTION_DEFINITON f1

#define TRAIT(name, f1)                                                        \
    struct name {                                                              \
        TRAIT_FTABLE(name, f1)                                                 \
        TRAIT_BODY(name, f1)                                                   \
    };
