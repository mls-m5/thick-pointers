#include "rawtraitimplementation.h"
#include "thickpointer.h"
#include "trait.h"
#include <iostream>

// Traits can be defined before or after the class is defined, it does not mater
Trait1(Movable,                 //
       (move, void, (int, int)) //
);

Trait2(ComplexMovable,           //
       (move, void, (int, int)), //
       (jump, void, (bool))      //
);

class Apa {
public:
    void move(int x, int y) {
        std::cout << "apa moves " << x << ", " << y << std::endl;
    };

    void jump(bool x) {
        std::cout << "apa jumps" << std::endl;
    }
};

class Bepa {
public:
    void move(int x, int y) {
        std::cout << "bepa moves " << x << ", " << y << std::endl;
    };

    void jump(bool x) {
        std::cout << "bepa jumps" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    {
        std::cout << "\nmacro version..." << std::endl;
        auto apa = Apa{};
        auto bepa = Bepa{};
        auto p = ThickPointer<Movable>(&apa);
        p.move(10, 20);

        p = &bepa;
        p.move(10, 30);
    }

    {
        std::cout << "\nmacro version 2..." << std::endl;
        auto apa = Apa{};
        auto bepa = Bepa{};
        auto p = ThickPointer<ComplexMovable>(&apa);
        p.move(10, 20);
        p.jump(true);

        p = &bepa;
        p.move(10, 30);
        p.jump(false);
    }

    {
        std::cout << "\nreference implementation...\n";
        auto apa = Apa{};
        auto bepa = Bepa{};

        auto p = ThickPointer<RawMovable>{&apa};

        p.move(10, 20);
        p.jump(true);

        p = &bepa;

        p.move(20, 30);
        p.jump(false);
    }

    return 0;
}
