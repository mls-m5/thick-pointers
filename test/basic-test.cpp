#include "rawtraitimplementation.h"
#include "trait.h"
#include <iostream>

// Traits can be defined before or after the class is defined, it does not
// matter
Trait1(Movable,                  //
       (move, void, (int, int),) //
);

Trait2(ComplexMovable,            //
       (move, void, (int, int),), //
       (jump, float, (bool),)     //
);

class Apa {
public:
    void move(int x, int y) {
        std::cout << "apa moves " << x << ", " << y << std::endl;
    };

    float jump(bool height) {
        float h = 1 + 2 * height;
        std::cout << "apa jumps " << h << std::endl;
        return h;
    }
};

class Bepa {
public:
    void move(int x, int y) {
        std::cout << "bepa moves " << x << ", " << y << std::endl;
    };

    float jump(bool height) {
        float h = 3 + 4 * height;
        std::cout << "bepa jumps " << h << std::endl;
        return h;
    }
};

void useMovable(ComplexMovable &m) {
    std::cout << "in function useMovable" << std::endl;
    m.move(10, 40);
    m.jump(true);
}

int main(int argc, char *argv[]) {
    {
        std::cout << "\nmacro version..." << std::endl;
        auto apa = Apa{};
        auto bepa = Bepa{};
        auto p = Movable{&apa};
        p.move(10, 20);

        p = &bepa;
        p.move(10, 30);
    }

    {
        std::cout << "\nmacro version 2..." << std::endl;
        auto apa = Apa{};
        auto bepa = Bepa{};
        auto p = ComplexMovable{&apa};
        p.move(10, 20);
        auto h = p.jump(true);
        std::cout << "returned value " << h << std::endl;

        p = &bepa;
        p.move(10, 30);
        p.jump(false);

        useMovable(p);
    }

    {
        std::cout << "\nreference implementation...\n";
        auto apa = Apa{};
        auto bepa = Bepa{};

        auto p = RawMovable{&apa};

        p.move(10, 20);
        p.jump(true);

        p = &bepa;

        p.move(20, 30);
        p.jump(false);
        auto h = p.jump(true);
        std::cout << "returned value " << h << std::endl;
    }

    return 0;
}
