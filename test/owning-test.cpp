// #include "referenceowningimplementation.h"
#include "trait.h"
// #include "rawtraitimplementation.h"
#include "rawtraitimplementation.h"
#include <iostream>

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

    auto m = makeUniqueMovable<SomethingThatMoves>();

    m.jump(true);
    m.move(10, 20);

    return 0;
}
