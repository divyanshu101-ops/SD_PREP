# Phase 1 Notes: JavaScript Function Basics

## 1. What is a function, and why do we use it?

A **function** is a reusable block of code that performs one specific task. You write it once and run it whenever you need it.

**Without a function** (repeating yourself):

```js
console.log("Hello, Rahul!");
console.log("Hello, Priya!");
console.log("Hello, Amit!");
```

**With a function:**

```js
function greet(name) {
  console.log("Hello, " + name + "!");
}

greet("Rahul");
greet("Priya");
greet("Amit");
```

### The DRY principle

**DRY = Don't Repeat Yourself.** If the same logic appears in many places, put it in one function.

- **Easier to maintain:** fix a bug in one place, not ten.
- **Easier to read:** `calculateTax(price)` explains itself.
- **Easier to test:** you can test one small piece at a time.

A function has two steps: **define it** (write it), then **call it** (run it).

```js
function sayHi() {          // define
  console.log("Hi!");
}
sayHi();                    // call -> prints "Hi!"
```

> **Common mistake:** `sayHi` without `()` does not run the function. It only refers to it. You need `sayHi()` to call it.

---

## 2. Function Declaration vs. Function Expression

These are the two most common ways to create a function. This is a very common interview question.

### Function Declaration

```js
function add(a, b) {
  return a + b;
}
```

### Function Expression

The function is created and stored in a variable.

```js
const add = function (a, b) {
  return a + b;
};
```

### The key difference: Hoisting

Before running your code, JavaScript scans it and sets up memory. This is called the **memory creation phase**.

- **Function declarations** are stored in memory completely, so you can call them **before** the line where they are written.
- **Function expressions** are only as hoisted as their variable is, so calling them early fails.

```js
// Declaration: works
greet("Divyanshu");              // "Hello Divyanshu"

function greet(name) {
  return "Hello " + name;
}
```

```js
// Expression with const: fails
sayHi("Divyanshu");              // ReferenceError: Cannot access 'sayHi' before initialization

const sayHi = function (name) {
  return "Hi " + name;
};
```

With `var` instead of `const`, the error is different:

```js
sayHi("Divyanshu");              // TypeError: sayHi is not a function

var sayHi = function (name) {
  return "Hi " + name;
};
```

`var` is hoisted with the value `undefined`, so JavaScript tries to call `undefined()`. With `let`/`const` the variable exists but is in the **Temporal Dead Zone (TDZ)** until its line runs.

| | Declaration | Expression |
|---|---|---|
| Syntax | `function f() {}` | `const f = function() {}` |
| Callable before its line? | Yes | No |
| Needs a name? | Yes | No (can be anonymous) |
| Common use | General-purpose functions | Callbacks, values, conditional creation |

---

## 3. Parameters vs. Arguments

- **Parameters** are the variable names in the function **definition**.
- **Arguments** are the actual values you pass when you **call** it.

```js
function introduce(name, age) {     // name, age -> PARAMETERS
  console.log(name + " is " + age);
}

introduce("Divyanshu", 21);         // "Divyanshu", 21 -> ARGUMENTS
```

JavaScript is relaxed about the count:

```js
function add(a, b) {
  return a + b;
}

add(2, 3);        // 5
add(2);           // NaN (b is undefined, and 2 + undefined = NaN)
add(2, 3, 4);     // 5 (the extra argument is ignored)
```

- Missing arguments become `undefined`.
- Extra arguments are ignored (though you can still access them, which you'll see in Phase 2).

---

## 4. The `return` statement

`return` does two things:

1. Sends a value back to whoever called the function.
2. **Immediately stops** the function.

```js
function square(n) {
  return n * n;
  console.log("This never runs");   // unreachable
}

const result = square(4);
console.log(result);                 // 16
```

### No `return` means `undefined`

```js
function noReturn() {
  const x = 5;
}

console.log(noReturn());   // undefined
```

### `console.log` vs. `return`

Beginners often mix these up.

```js
function a() { console.log(10); }   // only prints, returns undefined
function b() { return 10; }         // gives back a value you can use

const x = a();   // prints 10, x is undefined
const y = b();   // y is 10
```

Use `return` when you want to **use** the result later.

### Gotcha: never put the value on the next line

```js
function bad() {
  return
    5 + 5;
}
console.log(bad());   // undefined
```

JavaScript inserts a semicolon after a bare `return`. Always keep the value on the **same line** as `return`.

### Returning early

Returning early is a common pattern for validation:

```js
function divide(a, b) {
  if (b === 0) {
    return "Cannot divide by zero";
  }
  return a / b;
}
```

---

## 5. Default Parameters (ES6)

You can give a parameter a fallback value that is used when no argument is passed.

**Old way:**

```js
function greet(name) {
  name = name || "Guest";
  return "Hello, " + name;
}
```

**Modern way:**

```js
function greet(name = "Guest") {
  return "Hello, " + name;
}

greet("Divyanshu");   // "Hello, Divyanshu"
greet();              // "Hello, Guest"
```

### Important rules

**1. Defaults only apply for `undefined`**, not for `null`, `0`, or `""`:

```js
greet(undefined);   // "Hello, Guest"
greet(null);        // "Hello, null"
greet("");          // "Hello, "
```

**2. Defaults can use earlier parameters:**

```js
function createUser(name, username = name.toLowerCase()) {
  return { name, username };
}
createUser("Divyanshu");   // { name: "Divyanshu", username: "divyanshu" }
```

**3. Defaults are evaluated at call time**, so a new value is created on every call.

---

## 6. Scope Basics

**Scope** decides where a variable can be accessed.

| Scope | Meaning |
|---|---|
| **Global** | Declared outside any function, so accessible everywhere |
| **Function** | Declared inside a function, so accessible only inside it |
| **Block** | Declared inside `{ }` (if, for, etc.) with `let`/`const` |

```js
const globalVar = "I am global";

function demo() {
  const functionVar = "I am inside the function";
  console.log(globalVar);      // works
  console.log(functionVar);    // works
}

demo();
console.log(functionVar);      // ReferenceError
```

### `var` vs. `let` vs. `const`

```js
function test() {
  if (true) {
    var a = 1;     // function-scoped
    let b = 2;     // block-scoped
    const c = 3;   // block-scoped
  }
  console.log(a);  // 1
  console.log(b);  // ReferenceError
  console.log(c);  // ReferenceError
}
```

| | `var` | `let` | `const` |
|---|---|---|---|
| Scope | Function | Block | Block |
| Reassign? | Yes | Yes | No |
| Redeclare? | Yes | No | No |
| Hoisting | Hoisted as `undefined` | Hoisted, TDZ | Hoisted, TDZ |

**Rule of thumb:** use `const` by default, `let` when you need to reassign, and avoid `var`.

### Shadowing

An inner variable can have the same name as an outer one and will "shadow" it:

```js
const x = 10;

function show() {
  const x = 20;        // separate variable
  console.log(x);      // 20
}

show();
console.log(x);        // 10
```

---

## Quick Recap

- A function is a reusable block of code, and DRY is the reason to use one.
- **Declarations** are hoisted fully; **expressions** are not.
- **Parameters** are placeholders; **arguments** are actual values.
- `return` sends a value back and stops the function; no `return` means `undefined`.
- **Default parameters** kick in only for `undefined`.
- Prefer `const`/`let` (block scope) over `var` (function scope).

---

## Practice Problems

### Warm-up

1. Write a function `isEven(n)` that returns `true` or `false`.
2. Write `greet(name, greeting = "Hello")`.
3. Write `max(a, b)` that returns the larger number.

### Core

4. Write `factorial(n)` (use a loop for now).
5. Write `isPalindrome(str)`.
6. Write `countVowels(str)`.
7. Write `reverseString(str)`.
8. Write `sumArray(arr)` using a `for` loop.

### Concept checks (predict the output before running)

```js
// Q1
console.log(hello());
function hello() { return "hi"; }

// Q2
console.log(hey());
var hey = function () { return "hey"; };

// Q3
function test() { const a = 5; }
console.log(test());

// Q4
function f(x = 10) { return x; }
console.log(f(null), f(undefined), f());
```

### Answers

- Q1: `"hi"` (declaration is hoisted)
- Q2: `TypeError: hey is not a function`
- Q3: `undefined` (no return)
- Q4: `null 10 10`

### Mini project

Build a small calculator with `add`, `subtract`, `multiply`, and `divide` functions. Handle divide-by-zero and use a default parameter somewhere.
