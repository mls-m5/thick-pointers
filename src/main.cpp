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

// TODO: Hide this in some ugly macro... :'(
struct Movable {
    void (*move)(void *, int, int);

    template <typename T>
    Movable(T *)
        : move{reinterpret_cast<decltype(move)>(T::move)} {}
};

template <typename Trait>
struct ThickPointer {
    void *p;
    Trait trait;

    template <typename Type>
    ThickPointer(Type *p)
        : p{p}
        , trait{Trait{p}} {
    } // TODO: Statically create only one trait object per type

    template <typename Type>
    ThickPointer &operator=(Type *p) {
        this->p = p;
        this->trait = Trait{p};

        return *this;
    }

    void move(int x, int y) {
        trait.move(p, x, y);
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
