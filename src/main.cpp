#include <iostream>

class Apa {
public:
    static void move(Apa *self, int x, int y) {
        self->x = x;
        self->y = y;
        std::cout << "moving apa " << x << ", " << y << std::endl;
    };

private:
    int x = 0;
    int y = 0;
};

class Bepa {
public:
    static void move(Apa *self, int x, int y) {
        std::cout << "moving bepa " << x << ", " << y << std::endl;
    };
};

struct ThickPointerBase {
    void *p;
};

// TODO: Hide this in some ugly macro... :'(
struct Movable : virtual ThickPointerBase {
    void (*_move)(void *, int, int);

    template <typename T>
    Movable(T *p)
        : _move{reinterpret_cast<decltype(_move)>(T::move)} {
        this->p = p;
    }

    template <typename T>
    Movable &operator=(T *p) {
        _move = reinterpret_cast<decltype(_move)>(T::move);
        return *this;
    }

    void move(int x, int y) {
        _move(p, x, y);
    }
};

template <typename Trait>
struct ThickPointer : public Movable {
    template <typename Type>
    ThickPointer(Type *p)
        : Trait{p} {} // TODO: Statically create only one trait object per type

    template <typename Type>
    ThickPointer &operator=(Type *p) {
        Trait::operator=(p);
        return *this;
    }
};

// The theory does not work yet...
int main(int argc, char *argv[]) {
    std::cout << "hello there\n";

    auto apa = Apa{};
    auto bepa = Bepa{};

    auto p = ThickPointer<Movable>{&apa};

    p.move(10, 20);

    p = &bepa;

    p.move(20, 30);

    return 0;
}
