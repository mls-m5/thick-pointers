#pragma once

namespace tpimpl {

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

/// Create a single trait function table per class
template <typename Type, typename Trait, typename Table, typename... FuncS>
auto *functionTableInstance() {
    // I would prefere to not create this on the heap, but the compiler throws
    // mysterious type conversion error when not so I give up
    static auto table =
        new Table{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};

    return table;
}

} // namespace tpimpl

#define TRAIT_FTABLE_FUNCTION(name, ret, args)                                 \
    ret(tpimpl::FunctionMemberDummy::*name) args;

#define TRAIT_CONSTRUCTOR_FUNCTION(name, ret, args)                            \
    tpimpl::FunctionTypeStruct<decltype(FunctionTable::name),                  \
                               decltype(&T::name),                             \
                               &T::name>

#define TRAIT_FUNCTION_DEFINITION(name, ret, args)                             \
    template <typename... Args>                                                \
    void name(Args... a) {                                                     \
        return (p->*_ftable->name)(a...);                                      \
    }

#define Trait1(name, f1)                                                       \
    struct name {                                                              \
        struct FunctionTable {                                                 \
            TRAIT_FTABLE_FUNCTION f1;                                          \
        };                                                                     \
        tpimpl::FunctionMemberDummy *p = nullptr;                              \
        FunctionTable *_ftable = nullptr;                                      \
                                                                               \
        template <typename T>                                                  \
        name(T *p) {                                                           \
            _ftable = tpimpl::functionTableInstance<                           \
                T,                                                             \
                name,                                                          \
                FunctionTable,                                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f1>();                              \
            this->p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(p);      \
        }                                                                      \
                                                                               \
        TRAIT_FUNCTION_DEFINITION f1                                           \
    };

#define Trait2(name, f1, f2)                                                   \
    struct name {                                                              \
        struct FunctionTable {                                                 \
            TRAIT_FTABLE_FUNCTION f1;                                          \
            TRAIT_FTABLE_FUNCTION f2                                           \
        };                                                                     \
        tpimpl::FunctionMemberDummy *p = nullptr;                              \
        FunctionTable *_ftable = nullptr;                                      \
                                                                               \
        template <typename T>                                                  \
        name(T *p) {                                                           \
            _ftable = tpimpl::functionTableInstance<                           \
                T,                                                             \
                name,                                                          \
                FunctionTable,                                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f1,                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f2>();                              \
            this->p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(p);      \
        }                                                                      \
                                                                               \
        TRAIT_FUNCTION_DEFINITION f1 TRAIT_FUNCTION_DEFINITION f2              \
    };
