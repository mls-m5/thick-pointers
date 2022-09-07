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

// template <typename FunctionTable,
//           typename Trait,
//           typename Type,
//           typename... FuncS>
// auto *functionTableInstance() {
//     static auto instance =
//         FunctionTable{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
//     return &instance;
// }

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

template <typename Type, typename Trait, typename Table, typename... FuncS>
auto *ftStructInstance() {
    //    static auto table = Table{
    //        reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
    return new Table{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
}

template <typename... F>
using FunctionTable = std::tuple<F...>;

// TODO: Hide this in some ugly macro... :'(
struct Movable {
    using moveFT = void (FunctionMemberDummy::*)(int, int);
    using jumpFT = void (FunctionMemberDummy::*)(bool);

    struct FTStruct {
        void (FunctionMemberDummy::*move)(int, int);
        void (FunctionMemberDummy::*jump)(bool);
    };

    FunctionMemberDummy *p = nullptr;

    //    using FunctionTableT = FunctionTable<moveFT, jumpFT>;
    FTStruct *_ftable;

    template <typename T, typename Trait, typename... FuncS>
    void init(T *p) {
        _ftable =
            new FTStruct{reinterpret_cast<typename FuncS::FT>(FuncS::FP)...};
        _ftable = ftStructInstance<T, Trait, FTStruct, FuncS...>();
    }

    template <typename T>
    Movable(T *p) {
        init<T,
             Movable,
             FunctionTypeStruct<moveFT, decltype(&T::move), &T::move>,
             FunctionTypeStruct<jumpFT, decltype(&T::jump), &T::jump>>(p);
        this->p = reinterpret_cast<FunctionMemberDummy *>(p);
    }

    void move(int x, int y) {
        auto f = _ftable->move;
        (p->*f)(x, y);
    }

    void jump(bool x) {
        auto f = _ftable->jump;
        (p->*f)(x);
    }
};
