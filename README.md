# sdl_fungeoid

Fungeoid (Befuge-like) game written on C and SDL2

## Dependencies

```
sudo apt install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

## Code style

I have to remember to:

- Return zero on success and one on error.

- Set variables to some value on declaration, pointers to NULL.

- Set pointers to NULL when freeing memory.

- Use `static` for global variables and private functions.

- Can't define global variables on header files, see `COLOR_XX` for example

- For every struct with constructor:

    - Create a constructor `_create()` function that returns NULL on error.
        Remember to free the memory allocated inside the constructor on error.

    - Create a destructor function `_free()` that returns void, do not check for
        NULL pointer because `free()` can handle NULL pointers, but if the the
        struct has pointers I have to free them before checking for NULL pointer.

- Input handling:

    - `main.c` handles window and quit events, then, calls
        `input_handle_event()` so the input handles the touch/mouse/keyboard
        events.

    - `input_handle_event()` returns an `Input` struct, `main.c` passes the
        struct to the keyboard

    - If the keyboard returns KEYB_EVENT_NOT_HANDLED then `main.c` should pass
        the input to the element below, e.g. the HUD.

    - If the HUD doens not handle the input then the input is passed to the
        field.

## Android

```
sudo apt install openjdk-8-jdk ant android-sdk-platform-tools-common
```

Download `sdk-tools-linux-4333796`. Accept licenses:

```
env JAVA_HOME="/android/java-se-8u40-ri" /home/mbernardi/extra/async/android/sdk-tools-linux/tools/bin/sdkmanager --licenses
```

Install build tools, SDK and NDK for the target version. I had to use `--no_https`:

```
env JAVA_HOME="/home/mbernardi/extra/async/android/java-se-8u40-ri" /home/mbernardi/extra/async/android/sdk-tools-linux/tools/bin/sdkmanager --no_https "build-tools;26.0.0" "platforms;android-26"
env JAVA_HOME="/home/mbernardi/extra/async/android/java-se-8u40-ri" /home/mbernardi/extra/async/android/sdk-tools-linux/tools/bin/sdkmanager --no_https "ndk-bundle"
```

For compiling:

```
env ANDROID_HOME="/home/mbernardi/extra/async/android/sdk-tools-linux" JAVA_HOME="/home/mbernardi/extra/async/android/java-se-8u40-ri" ./gradlew installDebug
```

### Notes

I'm just copying and pasting the SDL sources into this repo so the Android build
works. I should update them manually.

Also I had to do modifications to `Android.mk` on `SDL_image`:

- Change to `SUPPORT_WEBP ?= false`.

- Delete `IMG_WIC.c       \`

## Acknowledgments

Font:

```
Inconsolata
Copyright (c) 2011, Raph Levien (firstname.lastname@gmail.com), Copyright (c) 2012, Cyreal (cyreal.org)
```

SDL: zlib license.

- https://www.willusher.io/pages/sdl2/

- http://lazyfoo.net/tutorials/SDL/index.php

## References

- https://wiki.libsdl.org/Android

- https://hg.libsdl.org/SDL/file/default/docs/README-android.md

## Other

```
#####  Color Palette by Paletton.com
#####  Palette URL: http://paletton.com/#uid=33t0E0kn9qlv8J4rfvFjlludJfm


*** Primary color:

   shade 0 = #2A6585 = rgb( 42,101,133) = rgba( 42,101,133,1) = rgb0(0.165,0.396,0.522)
   shade 1 = #1089CB = rgb( 16,137,203) = rgba( 16,137,203,1) = rgb0(0.063,0.537,0.796)
   shade 2 = #1F73A0 = rgb( 31,115,160) = rgba( 31,115,160,1) = rgb0(0.122,0.451,0.627)
   shade 3 = #2E576D = rgb( 46, 87,109) = rgba( 46, 87,109,1) = rgb0(0.18,0.341,0.427)
   shade 4 = #2E434E = rgb( 46, 67, 78) = rgba( 46, 67, 78,1) = rgb0(0.18,0.263,0.306) // Background

   #677278 // Lines

*** Secondary color (1):

   shade 0 = #D2B23A = rgb(210,178, 58) = rgba(210,178, 58,1) = rgb0(0.824,0.698,0.227)
   shade 1 = #FFCB07 = rgb(255,203,  7) = rgba(255,203,  7,1) = rgb0(1,0.796,0.027)
   shade 2 = #FCD025 = rgb(252,208, 37) = rgba(252,208, 37,1) = rgb0(0.988,0.816,0.145)
   shade 3 = #AB9644 = rgb(171,150, 68) = rgba(171,150, 68,1) = rgb0(0.671,0.588,0.267) // Select_1
   shade 4 = #7A6F46 = rgb(122,111, 70) = rgba(122,111, 70,1) = rgb0(0.478,0.435,0.275)

*** Secondary color (2):

   shade 0 = #D23A3A = rgb(210, 58, 58) = rgba(210, 58, 58,1) = rgb0(0.824,0.227,0.227)
   shade 1 = #FF0707 = rgb(255,  7,  7) = rgba(255,  7,  7,1) = rgb0(1,0.027,0.027)
   shade 2 = #FC2525 = rgb(252, 37, 37) = rgba(252, 37, 37,1) = rgb0(0.988,0.145,0.145)
   shade 3 = #AB4444 = rgb(171, 68, 68) = rgba(171, 68, 68,1) = rgb0(0.671,0.267,0.267) // Select_2
   shade 4 = #7A4646 = rgb(122, 70, 70) = rgba(122, 70, 70,1) = rgb0(0.478,0.275,0.275)


#####  Generated by Paletton.com (c) 2002-2014
```

## Themes

- Graphic
- Befunge chars
- Befunge integers
- Fish chars
- Fish integers

## Fungeoid commands

```
Movements
---------

SPACE:

RIGHT: >
LEFT: <
DOWN: v
UP: ^

RND: ? (93) x (fish)
BRIDGE: # (93) ! (fish)
STOP: @ (93) ; (fish)

Operators
---------

ADD: +
SUB: -
MUL: *
INTDIV: / (93)
// FLTDIV: , (fish)
MOD: %
NOT: ! (93)
GT: ` (93) ( (fish)
LT: , (FUN!geoid) ) (fish)

Stack
-----

DUP: :
SWP: \ (93) $ (fish)
POP: $ (93) ~ (fish)

Conditional movements
---------------------

HIF: _ (93)
VIF: | (93)
STR: " (93) " (fish) ' (fish)
ITER: k (98)

Input
-----

0: 0
1: 1
...
9: 9
A: a
B: b
...
F: f
INTIN: & (93)
CHARIN: ~ (93)

Output
------

INTOUT: . (93) n (fish)
CHAROUT: , (93) o (fish)

Write and get from world
------------------------

GET g
PUT p
FETCH: ' (98)
STORE: s (98)
```

### Keyboard groups

Values:

```
9 8 7 a b c
6 5 4 d e f
3 2 1
  0
```

Movement and IO

```
< v ^ > # ? @
& ~ . , g p ' s
```

Operations and conditionals
```
+ - * / % ! ` ,
: \ $ _ | " k
```

### Original commands

- https://esolangs.org/wiki/

Befunge:

```
+ Addition: Pop two values a and b, then push the result of a+b
- Subtraction: Pop two values a and b, then push the result of b-a
* Multiplication: Pop two values a and b, then push the result of a*b
/ Integer division: Pop two values a and b, then push the result of b/a, rounded down. According to the specifications, if a is zero, ask the user what result they want.
% Modulo: Pop two values a and b, then push the remainder of the integer division of b/a.
! Logical NOT: Pop a value. If the value is zero, push 1; otherwise, push zero.
` Greater than: Pop two values a and b, then push 1 if b>a, otherwise zero.
> PC direction right
< PC direction left
^ PC direction up
v PC direction down
? Random PC direction
_ Horizontal IF: pop a value; set direction to right if value=0, set to left otherwise
| Vertical IF: pop a value; set direction to down if value=0, set to up otherwise
" Toggle stringmode (push each character's ASCII value all the way up to the next ")
: Duplicate top stack value
\ Swap top stack values
$ Pop (remove) top stack value and discard
. Pop top of stack and output as integer
, Pop top of stack and output as ASCII character
# Bridge: jump over next command in the current direction of the current PC
g A "get" call (a way to retrieve data in storage). Pop two values y and x, then push the ASCII value of the character at that position in the program. If (x,y) is out of bounds, push 0
p A "put" call (a way to store a value for later use). Pop three values y, x and v, then change the character at the position (x,y) in the program to the character with ASCII value v
& Get integer from user and push it
~ Get character from user and push it
@ End program
0 – 9 Push corresponding number onto the stack
```

Befunge98 instructions I like:

```
a Push 10
b Push 11
c Push 12
d Push 13
e Push 14
f Push 15
k Iterate: execute next instruction now, 0 or n+1 times (k does not skip IP unless 0)
' Fetch: push to stack next character and bridge over it. "Q", is quivalent to 'Q,
s Store: inverse of fetch.
```

Fish commands I like:

```
> PC direction right
< PC direction left
^ PC direction up
v PC direction down
\ Mirror
/ Mirror
_ Mirror
| Mirror
# Mirror
x Random PC direction
! Trampoline
? Conditional trampoline - pop one value off the stack. The next instruction is only executed if the popped value is non-zero.
. Jump - pop y and x off the stack, and move the IP to (x,y) in the codebox. The current direction is retained. Note that you have to jump to the cell before the instructions you want to execute, as the IP will move one position next tick before executing.
0 – 9 Push corresponding number onto the stack
a Push 10
b Push 11
c Push 12
d Push 13
e Push 14
f Push 15
+ Addition: Pop two values a and b, then push the result of a+b
- Subtraction: Pop two values a and b, then push the result of b-a
* Multiplication: Pop two values a and b, then push the result of a*b
, Float division: float division (meaning 94,n; outputs 2.25 and not 2). Division by 0 raises an error.
% Modulo: Pop two values a and b, then push the remainder of the integer division of b/a.
= Equals. Pop x and y off the stack, and push 1 if y = x, and 0 otherwise.
) ( Greater than and less than, respectively. Pop x and y off the stack, and push 1 if y operator x, and 0 otherwise.
' "	Single and double quote - enable string parsing. String parsing pushes every character found to the stack until it finds a closing quote.
l Push the length of the stack onto the stack.
: Duplicate the top value on the stack.
$ Swap the top two values on the stack
@ Swap the top three values on the stack, shifting them rightwards (e.g. if your stack is 1,2,3,4, calling @ results in 1,4,2,3)
~ Remove the top value from the stack.
o n Pop and output as a character and a number, respectively. Output is written to stdout.
i Read one character from stdin and push it to the stack. The character should not be shown when read from console. When no more input is available, -1 is pushed.
& Pop the top value off the stack and put it in the register. Calling & again will take the value in the register and put it back on the stack. See The register.
g Pop y and x off the stack, and push the value at x,y in the codebox. Empty cells are equal to 0.
p Pop y, x, and v off the stack, and change the value at x,y to v. E.g. 123p puts 1 at 2,3 in the codebox.
; End execution.
```

Fish commands I don't like:

```
[ Pop x off the stack and create a new stack, moving x values from the old stack onto the new one. See Stacks.
] Remove the current stack, moving its values to the top of the underlying stack.
} { Shift the entire stack to the right and left, respectively. (e.g. when having 1,2,3,4, calling } results in 4,1,2,3 while { results in 2,3,4,1)
r Reverse the stack.
```
