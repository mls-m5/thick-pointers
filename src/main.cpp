#include "rawtraitimplementation.h"
#include "thickpointer.h"
#include <iostream>

class Apa {
public:
    void move(int x, int y) {
        x = x;
        y = y;
        std::cout << "moving apa " << x << ", " << y << std::endl;
    };

    void jump(bool x) {
        std::cout << "apa jumps" << std::endl;
    }

private:
    int x = 0;
    int y = 0;
};

class Bepa {
public:
    void move(int x, int y) {
        std::cout << "moving bepa " << x << ", " << y << std::endl;
    };

    void jump(bool x) {
        std::cout << "bepa jumps" << std::endl;
    }
};

TRAIT1(Movable,                 //
       (move, void, (int, int)) //
)

// The theory does not work yet...
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
