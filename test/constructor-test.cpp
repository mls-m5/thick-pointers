#include "trait.h"
#include <iostream>

Trait1(Movable,                 //
       (move, void, (int, int), ) //
);

struct Apa {
    void move(int x, int y) {
        std::cout << "x: " << x << " y: " << y << std::endl;
    }
};

int main() {
    auto movable = Movable{}; // Empty constructor

    auto apa = Apa{};

    movable = &apa; // Default constructor

    std::cout << "x\n";
}
