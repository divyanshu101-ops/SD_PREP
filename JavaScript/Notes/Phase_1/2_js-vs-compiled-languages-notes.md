# Hoisting Recap + JavaScript vs Compiled Languages (C++, Python)

## 1. Hoisting & Variable Lifecycle — `var` vs `let`/`const` (with examples)

### `var` — Function/Global Scoped

```javascript
console.log(a);   // undefined (not an error)
var a = 10;
console.log(a);   // 10
```

**What actually happens (Memory Creation Phase):**
```javascript
// Engine's internal view before execution starts:
var a = undefined;

// Then execution phase runs your actual lines:
console.log(a);   // undefined
a = 10;
console.log(a);   // 10
```

**Output:**
```
undefined
10
```

`var` is hoisted **and pre-filled** with `undefined`, so accessing it early doesn't crash — it just gives you a placeholder value.

---

### `let` / `const` — Block Scoped + Temporal Dead Zone (TDZ)

```javascript
console.log(b);   // ReferenceError
let b = 20;
```

**Output:**
```
Uncaught ReferenceError: Cannot access 'b' before initialization
```

`b` is hoisted into memory but **not initialized** — the zone between the top of the block and the actual `let b = 20;` line is the **TDZ**. Touching it throws, it doesn't silently return `undefined`.

```javascript
{
  // TDZ for c starts here
  console.log(typeof c); // ReferenceError, even with typeof!
  let c = 5;
  // TDZ ends here
}
```

**Output:**
```
Uncaught ReferenceError: Cannot access 'c' before initialization
```

Note: normally `typeof undeclaredVar` safely returns `"undefined"` for a variable that was never declared at all — but inside the TDZ, it still throws, because the engine already knows `c` exists in this scope (it's hoisted), it just isn't initialized yet.

---

## 2. JavaScript vs Compiled Languages — Core Difference

| | **C++** | **Python** | **JavaScript** |
|---|---|---|---|
| Type checking | Static (checked before running) | Dynamic (checked while running) | Dynamic (checked while running) |
| Needs a separate compile step? | Yes (`g++ file.cpp -o app`, then run `./app`) | No — compiles to bytecode internally, then runs immediately | No — parses + compiles to bytecode internally (V8), then runs immediately |
| Syntax errors | Caught at compile time — program never runs | Caught before execution starts — whole file is parsed first | Caught before execution starts — whole file is parsed first |
| Type errors (e.g. wrong type used) | Caught at compile time — program never runs | Only caught at runtime, when that exact line executes | Only caught at runtime, when that exact line executes |

**Key nuance people get wrong:** JavaScript and Python *do* read and parse the **entire file** before running anything (that's how hoisting even works). So a **syntax error anywhere** in a JS or Python file stops the *entire* program from running — even code above the error. The real difference from C++ is about **type errors**, not syntax errors: C++ catches wrong-type usage during compilation, while JS/Python only catch it when the engine actually reaches and executes that specific line.

---

## 3. Example — Syntax Error (all three fail before running anything)

**C++:**
```cpp
#include <iostream>
int main() {
    std::cout << "Hello"
    return 0;   // missing semicolon above
}
```
**Output:**
```
error: expected ';' before 'return'
```
(No compiled executable is produced. Nothing runs.)

**Python:**
```python
print("Hello")
print("World"
print("!")
```
**Output:**
```
SyntaxError: '(' was never closed
```
(Not even the first `print("Hello")` runs.)

**JavaScript:**
```javascript
console.log("Hello");
console.log("World"
console.log("!");
```
**Output:**
```
SyntaxError: missing ) after argument list
```
(Not even the first `console.log("Hello")` runs — V8 parses the whole file into an AST first, and this file never becomes valid syntax.)

---

## 4. Example — Type Error (this is where the real difference shows)

**C++ (statically typed — caught before the program ever runs):**
```cpp
#include <iostream>
int main() {
    int x = 5;
    std::cout << "Start\n";
    x = "hello";        // type mismatch: can't assign string to int
    std::cout << "End\n";
}
```
**Output:**
```
error: invalid conversion from 'const char*' to 'int'
```
Compilation fails. **Nothing runs at all** — not even `"Start"` gets printed, because the compiler rejects the whole program before it ever becomes an executable.

**Python (dynamically typed — caught only when that line runs):**
```python
x = 5
print("Start")
x = x + "hello"   # error: can't add int + str
print("End")
```
**Output:**
```
Start
Traceback (most recent call last):
  File "app.py", line 3, in <module>
    x = x + "hello"
TypeError: unsupported operand type(s) for +: 'int' and 'str'
```
`"Start"` **does** print, because Python runs line by line and only discovers the type error when it actually reaches that line. `"End"` never prints, because the program crashed before reaching it.

**JavaScript (dynamically typed — same idea, but often doesn't even crash):**
```javascript
let x = 5;
console.log("Start");
x = x + "hello";   // JS auto-converts instead of throwing!
console.log(x);
console.log("End");
```
**Output:**
```
Start
5hello
End
```
This is a key JS quirk: instead of throwing a type error like Python, JS's `+` operator **coerces** the number into a string and concatenates. To actually see a JS runtime crash from a type problem, you need something like:

```javascript
let y = 5;
console.log("Start");
y();                // y is a number, not a function
console.log("End");
```
**Output:**
```
Start
Uncaught TypeError: y is not a function
```
Same pattern as Python: `"Start"` prints, then it crashes mid-execution, `"End"` never runs.

---

## 5. Summary — What Actually Differs

1. **C++**: two separate steps — *compile* (type-checks everything, produces an executable or fails outright) then *run*. A type error anywhere means **zero output**, ever.
2. **Python**: one step from the user's view, but internally parses the whole file first (syntax errors block everything), then executes top-to-bottom, checking types **only as each line runs**. Output before the crash point still prints.
3. **JavaScript**: same shape as Python (parse whole file → execute line by line, checking types at runtime) — but JS is more "forgiving": many operations that would be type errors in Python (like `number + string`) are silently type-coerced in JS instead of crashing. You only get a runtime `TypeError` for things like calling a non-function or reading a property of `undefined`/`null`.
