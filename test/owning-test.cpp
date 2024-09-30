// #include "referenceowningimplementation.h"
#include "trait.h"
// #include "rawtraitimplementation.h"
#include "rawtraitimplementation.h"
#include "trait.h"
#include <iostream>

Trait2(Movable,                       //
       (move, void, (int x, int y),), //
       (jump, void, (bool), )         //
);

struct SomethingThatMoves {
    void move(int x, int y) {
        std::cout << "moved " << x << "," << y << std::endl;
    }

    float jump(bool arg) {
        std::cout << "jumping...\n";
        return true;
    }

    ~SomethingThatMoves() {
        std::cout << "destroyed" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    // Reference implementation
    {
        auto m = makeUniqueMovable<SomethingThatMoves>();

        m.jump(true);
        m.move(10, 20);

        auto m2 = std::move(m);

        m2.jump(true);
        m2.move(10, 20);
    }

    {
        auto m = Movable_Owning{new SomethingThatMoves};

        m.jump(true);
        m.move(10, 20);

        auto m2 = std::move(m);

        m2.jump(true);
        m2.move(10, 20);
    }

    return 0;
}
