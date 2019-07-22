# Fungeoid commands

- https://esolangs.org/wiki/

## Befunge:

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

## Befunge98 instructions I like

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

## Fish commands I like

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

## Fish commands I don't like:

```
[ Pop x off the stack and create a new stack, moving x values from the old stack onto the new one. See Stacks.
] Remove the current stack, moving its values to the top of the underlying stack.
} { Shift the entire stack to the right and left, respectively. (e.g. when having 1,2,3,4, calling } results in 4,1,2,3 while { results in 2,3,4,1)
r Reverse the stack.
```
