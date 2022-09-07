# thick-pointers in c++

An experiment to implement Rust like thick pointers in c++.

## Background

Rust and C++ differs in how dynamic dispatch is handled. In c++ each class
with virtual functions defined on the heap get a pointer to its `virtual table`
that keeps track of what function is associated with that instance.

In rust the pointer itself contains a pointer to the table containing the
virtual functions. That is where the name `thick-pointers` comes from.

Both methods have their pros and cons, for this experiment I wanted to know
wether it would be possible to implement thick pointers in c++ in a way
that would make them practically usable.

## Goals

- There should not be more redirects than Rust-code in the function calls
- The size of a thick pointer should only be that of two normal pointers
- Each Trait/class combination should only have one function table in memory

## Result

It was possible to implement the technology behind thick pointers, but
because of limitations in how c++ generates code I could not find a way
to create the Trait definitions (that is the part that the programmer will use)
without using Macros. This will probably not change any time soon, but if
you are willing to clutter your code with macros it is now possible to use
rust-like dynamic dispatching without using virtual functions.

It is also probably possible to do more macro magick to have a single trait
function, but that is more than I want to mess around with macros.

## Usage

Create a trait, that defines a set of functions that you want to call
```c++
Trait2(Movable,                  //
       (move, void, (int, int)), //
       (jump, void, (bool))      //
);
```

The comment markers at the end of the line is to force clang-format to do line
breaks.

After that define one or more classes that implements your functions:

```c++
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
    // Add another implementation
    // void move(int x, int y) { ... }
    // void jump(bool x) { ... }
};
```

Notice how none of these functions are `virtual`.

Then start using the traits. Notice how you do not need to define that the trait
`Movable` and the classes `Apa` and `Bepa` relates to each other. This is done
with templating, kind of similar to how Golang does.

```c++

int main(int argc, char *argv[]) {
    auto apa = Apa{};
    auto bepa = Bepa{};
    
    auto p = Movable(&apa);
    p.move(10, 20);
    p.jump(true);

    p = &bepa; // Reassign and use trait that uses Bepas functions instead
    p.move(10, 30);
    p.jump(false);
}
```

Example output:

```
apa moves 10, 20
apa jumps
bepa moves 10, 30
bepa jumps
```

You could also send the pointer to functions if you want to do some usable
dynamic dispatching with them. The size of each pointer only double of
that of a raw c++ pointer.

```c++
void useMovable(Movable m) {
    m.move(1, 2);
    ...
}
```

## Non-goals

Only dynamic dispatch is considered at this time, and not memory management.
There is many other features that differs between c++ and rust, such as borrow-
checking and much more. That is not in the scope of this project.
