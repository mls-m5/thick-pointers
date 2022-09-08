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
struct FunctionTableInstance {
    // I would prefere to not create this on the heap, but the compiler throws
    // mysterious type conversion error when not so I give up
    const static inline Table *table =
        new Table{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
};

} // namespace tpimpl

#define TRAIT_FTABLE_FUNCTION(name, ret, args)                                 \
    ret(tpimpl::FunctionMemberDummy::*name) args;

#define TRAIT_CONSTRUCTOR_FUNCTION(name, ret, args)                            \
    tpimpl::FunctionTypeStruct<decltype(FunctionTable::name),                  \
                               decltype(&T::name),                             \
                               &T::name>

#define TRAIT_FUNCTION_DEFINITION(name, ret, args)                             \
    template <typename... Args>                                                \
    auto name(Args... a) {                                                     \
        return (_p->*_ftable->name)(a...);                                     \
    }

#define Trait1(name, f1)                                                       \
    class name {                                                               \
        struct FunctionTable {                                                 \
            TRAIT_FTABLE_FUNCTION f1;                                          \
        };                                                                     \
        tpimpl::FunctionMemberDummy *_p = nullptr;                             \
        const FunctionTable *_ftable = nullptr;                                \
                                                                               \
    public:                                                                    \
        template <typename T>                                                  \
        name(T *_p) {                                                          \
            _ftable = tpimpl::FunctionTableInstance<                           \
                T,                                                             \
                name,                                                          \
                FunctionTable,                                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f1>::table;                         \
            this->_p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(_p);    \
        }                                                                      \
                                                                               \
        TRAIT_FUNCTION_DEFINITION f1                                           \
    };

#define Trait2(name, f1, f2)                                                   \
    class name {                                                               \
        struct FunctionTable {                                                 \
            TRAIT_FTABLE_FUNCTION f1;                                          \
            TRAIT_FTABLE_FUNCTION f2                                           \
        };                                                                     \
        tpimpl::FunctionMemberDummy *_p = nullptr;                             \
        const FunctionTable *_ftable = nullptr;                                \
                                                                               \
    public:                                                                    \
        template <typename T>                                                  \
        name(T *_p) {                                                          \
            _ftable = tpimpl::FunctionTableInstance<                           \
                T,                                                             \
                name,                                                          \
                FunctionTable,                                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f1,                                 \
                TRAIT_CONSTRUCTOR_FUNCTION f2>::table;                         \
            this->_p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(_p);    \
        }                                                                      \
                                                                               \
        TRAIT_FUNCTION_DEFINITION f1 TRAIT_FUNCTION_DEFINITION f2              \
    };
