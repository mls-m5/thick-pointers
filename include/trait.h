#pragma once
#include <array>
#include <utility>

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

// Used to send template parameters in larger packs
template <typename Type>
struct FunctionDestructorStruct {
    using FT = void (*)(void *);
    using OFT = void (*)(void *);
    constexpr static inline OFT FP = [](void *ptr) {
        auto t = reinterpret_cast<Type *>(ptr);
        delete t;
    };
};

/// Create a single trait function table per class
template <typename Type, typename Trait, typename Table, typename... FuncS>
struct FunctionTableInstance {
    // I would prefere to not create this like his, but the compiler throws
    // mysterious type conversion error when trying to compiling without using
    // new not I give up
    static inline std::array<char, sizeof(Table)> data = {};

    const static inline Table *table = new (data.data())
        Table{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
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
    auto name(Args &&...a) {                                                   \
        return (_p->*_ftable->name)(std::forward<Args>(a)...);                 \
    }

#define TRAIT_INTERNAL(name, TABLE, CONSTRUCTOR, FDEFINITION)                  \
    class name {                                                               \
        struct FunctionTable {                                                 \
            void (*destructor)(void *ptr);                                     \
            TABLE                                                              \
        };                                                                     \
        tpimpl::FunctionMemberDummy *_p = nullptr;                             \
        const FunctionTable *_ftable = nullptr;                                \
                                                                               \
    public:                                                                    \
        name() = default;                                                      \
                                                                               \
        template <typename T>                                                  \
        name(T *p) {                                                           \
            _ftable = tpimpl::FunctionTableInstance<                           \
                T,                                                             \
                name,                                                          \
                FunctionTable,                                                 \
                tpimpl::FunctionDestructorStruct<T>,                           \
                CONSTRUCTOR>::table;                                           \
            this->_p = reinterpret_cast<tpimpl::FunctionMemberDummy *>(p);     \
        }                                                                      \
                                                                               \
        FDEFINITION                                                            \
        friend class name##_Owning;                                            \
    };                                                                         \
    class name##_Owning : public name {                                        \
    public:                                                                    \
        template <typename T>                                                  \
        name##_Owning(T *p)                                                    \
            : name{p} {}                                                       \
                                                                               \
        name##_Owning(name##_Owning &&other) {                                 \
            _p = other._p;                                                     \
            _ftable = other._ftable;                                           \
            other._p = nullptr;                                                \
            _ftable = nullptr;                                                 \
        }                                                                      \
                                                                               \
        name##_Owning(const name##_Owning &other) = delete;                    \
                                                                               \
        name##_Owning &operator=(name##_Owning &&other) {                      \
            _p = other._p;                                                     \
            other._p = nullptr;                                                \
            return *this;                                                      \
        }                                                                      \
                                                                               \
        name##_Owning &operator=(const name##_Owning &other) = delete;         \
                                                                               \
        ~name##_Owning() {                                                     \
            if (_p) {                                                          \
                _ftable->destructor(_p);                                       \
            }                                                                  \
        }                                                                      \
    };

#define TRAIT_EXPAND(...) __VA_ARGS__

#define Trait1(name, f1)                                                       \
    TRAIT_INTERNAL(name, TRAIT_FTABLE_FUNCTION f1;                             \
                   ,                                                           \
                   TRAIT_EXPAND(TRAIT_CONSTRUCTOR_FUNCTION f1),                \
                   TRAIT_FUNCTION_DEFINITION f1)

#define Trait2(name, f1, f2)                                                   \
    TRAIT_INTERNAL(name, TRAIT_FTABLE_FUNCTION f1; TRAIT_FTABLE_FUNCTION f2;   \
                   ,                                                           \
                   TRAIT_EXPAND(TRAIT_CONSTRUCTOR_FUNCTION f1,                 \
                                TRAIT_CONSTRUCTOR_FUNCTION f2),                \
                   TRAIT_FUNCTION_DEFINITION f1 TRAIT_FUNCTION_DEFINITION f2)

#define Trait3(name, f1, f2, f3)                                               \
    TRAIT_INTERNAL(name, TRAIT_FTABLE_FUNCTION f1; TRAIT_FTABLE_FUNCTION f2;   \
                   TRAIT_FTABLE_FUNCTION f3;                                   \
                   ,                                                           \
                   TRAIT_EXPAND(TRAIT_CONSTRUCTOR_FUNCTION f1,                 \
                                TRAIT_CONSTRUCTOR_FUNCTION f2,                 \
                                TRAIT_CONSTRUCTOR_FUNCTION f3),                \
                   TRAIT_FUNCTION_DEFINITION f1 TRAIT_FUNCTION_DEFINITION f2   \
                       TRAIT_FUNCTION_DEFINITION f3)
