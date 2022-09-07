#pragma once
#include <tuple>

// The actual pointer type
template <typename Trait>
struct ThickPointer : public Trait {
    template <typename Type>
    ThickPointer(Type *p)
        : Trait{p} {}
};
