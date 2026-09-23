# Phase 1: Variables, Keywords, Data Types & Memory States — Complete Notes (JavaScript Only)

## 1. Variable Declaration Keywords — `var`, `let`, `const`

### `var` — Function Scoped

```javascript
function test() {
  if (true) {
    var x = 10;
  }
  console.log(x); // 10 — accessible outside the if-block
}
test();
```
**Output:**
```
10
```
`var` ignores block boundaries (`{ }` from `if`, `for`, etc.) — it's only confined by the nearest **function** (or global scope if there's no function).

**Re-declaration is allowed (and this is the dangerous part):**
```javascript
var age = 25;
var age = 30;   // no error — silently overwrites
console.log(age);
```
**Output:**
```
30
```
In a large file, if two different developers accidentally reuse the same variable name with `var`, the second one silently overwrites the first with **no warning at all**. This is exactly why `var` is avoided in modern backend code.

---

### `let` — Block Scoped

```javascript
if (true) {
  let y = 10;
  console.log(y); // 10
}
console.log(y); // ReferenceError: y is not defined
```
**Output:**
```
10
Uncaught ReferenceError: y is not defined
```
`let` respects `{ }` block boundaries — it does not leak out like `var` does.

**Re-assignment allowed, re-declaration NOT allowed (same scope):**
```javascript
let score = 50;
score = 80;          // fine — this is re-assignment
console.log(score);  // 80

let score = 100;      // SyntaxError — this is re-declaration
```
**Output:**
```
80
Uncaught SyntaxError: Identifier 'score' has already been declared
```

**TDZ recap:**
```javascript
console.log(z); // ReferenceError
let z = 5;
```
**Output:**
```
Uncaught ReferenceError: Cannot access 'z' before initialization
```

---

### `const` — Block Scoped, Immutable Binding

```javascript
const PI = 3.14;
PI = 3.14159;   // TypeError — reassignment forbidden
```
**Output:**
```
Uncaught TypeError: Assignment to constant variable.
```

**Important nuance — `const` locks the *binding*, not the *contents*:**
```javascript
const user = { name: "Divyanshu", age: 22 };
user.age = 23;         // allowed — mutating a property, not reassigning the variable
console.log(user);

user = { name: "New" }; // TypeError — this IS reassignment of the variable itself
```
**Output:**
```
{ name: 'Divyanshu', age: 23 }
Uncaught TypeError: Assignment to constant variable.
```
`const` only guarantees that the variable `user` will always point to the *same object in the heap*. It says nothing about whether that object's internal properties can change — they can, freely, through methods like `.push()`, `.pop()`, or direct property assignment.

```javascript
const numbers = [1, 2, 3];
numbers.push(4);       // allowed — mutating the array in place
console.log(numbers);  // [1, 2, 3, 4]

numbers = [5, 6, 7];    // TypeError — reassigning the variable itself
```
**Output:**
```
[ 1, 2, 3, 4 ]
Uncaught TypeError: Assignment to constant variable.
```

---

## 2. JavaScript Data Types

JavaScript splits data into **Primitives** and **Reference (Non-Primitive) Types**.

### Primitives — stored on the Stack, immutable

```javascript
let num = 25.5;          // Number — both integers and decimals, 64-bit floating point
let str = "Hello";        // String — text
let flag = true;          // Boolean — true / false
let notAssigned;          // Undefined — declared, no value given
let empty = null;         // Null — intentional "nothing"
let id = Symbol("id");    // Symbol — unique, immutable identifier
let big = 9007199254740993n; // BigInt — beyond Number.MAX_SAFE_INTEGER (2^53 - 1)
```

**Why "immutable" for primitives — example:**
```javascript
let s = "hello";
s.toUpperCase();   // this does NOT change s
console.log(s);    // "hello" — original string is untouched
console.log(s.toUpperCase()); // "HELLO" — a brand new string is returned instead
```
**Output:**
```
hello
HELLO
```
Any "modification" of a string actually creates and returns a **new** string in memory — the original primitive value itself can never be altered in place.

**The `typeof null` bug:**
```javascript
console.log(typeof null); // "object"
```
**Output:**
```
object
```
This is a well-known bug from JavaScript's very first (1995) implementation, kept in the language forever for backward compatibility — changing it now would break too much existing code across the web. `null` is still a primitive; `typeof` just reports it incorrectly.

**Number vs BigInt limit:**
```javascript
console.log(Number.MAX_SAFE_INTEGER);      // 9007199254740991
console.log(Number.MAX_SAFE_INTEGER + 1);  // 9007199254740992 (still looks ok)
console.log(Number.MAX_SAFE_INTEGER + 2);  // 9007199254740992 (WRONG — precision lost)

let safe = 9007199254740993n; // BigInt — exact, no precision loss
console.log(safe);
```
**Output:**
```
9007199254740991
9007199254740992
9007199254740992
9007199254740993n
```

---

### Reference (Non-Primitive) Types — stored on the Heap, mutable

```javascript
let person = { name: "Divyanshu", role: "Backend Dev" }; // Object
let skills = ["Node.js", "System Design", "AI"];          // Array (technically an Object)
function greet() { return "Hi"; }                          // Function — a "first-class citizen"
```

**"First-class citizen" means functions can be treated like any other value:**
```javascript
function sayHi() { return "Hi!"; }

let ref = sayHi;              // assigned to a variable
console.log(ref());           // called through the new name

function callTwice(fn) {      // passed as an argument
  return fn() + " " + fn();
}
console.log(callTwice(sayHi));

function makeGreeter() {      // returned from another function
  return function () { return "Hello from inside!"; };
}
console.log(makeGreeter()());
```
**Output:**
```
Hi!
Hi! Hi!
Hello from inside!
```

**Mutability + shared reference — the classic gotcha:**
```javascript
let obj1 = { count: 1 };
let obj2 = obj1;        // obj2 does NOT get a copy — it points to the same heap object
obj2.count = 99;
console.log(obj1.count); // 99 — obj1 changed too!
```
**Output:**
```
99
```

---

## 3. Dynamic Typing in JavaScript [Interview Important]

```javascript
let value = 42;
console.log(typeof value); // "number"

value = "now a string";
console.log(typeof value); // "string"

value = true;
console.log(typeof value); // "boolean"
```
**Output:**
```
number
string
boolean
```
No type is ever declared upfront — the V8 engine checks the value on the right side of `=` at the moment of assignment and labels the variable's *current* type accordingly. The same variable can hold completely different types across its lifetime, which is the opposite of a statically typed language like C++, where a variable's type is fixed forever the moment it's declared (`int x = 5;` — `x` can never become a string).

---

## 4. Memory States — Uninitialized vs. Undefined vs. Null

| State | Applies to | How it happens | Accessing it does |
|---|---|---|---|
| **Uninitialized** | `let`, `const` | Hoisted into memory, but no value bound yet — sits in the TDZ | Throws `ReferenceError` |
| **Undefined** | `var`, or any variable declared with no value | Engine auto-assigns this placeholder during Memory Creation Phase | Returns `undefined`, no error |
| **Null** | Any variable, any keyword | Developer manually writes `= null` | Returns `null`, no error |

**All three side by side:**
```javascript
console.log(a); // "uninitialized" state → ReferenceError
let a = 1;

console.log(b); // "undefined" state → prints undefined, no crash
var b = 2;

let c = null;   // "null" state → developer explicitly says "empty on purpose"
console.log(c);
```
**Output:**
```
Uncaught ReferenceError: Cannot access 'a' before initialization
undefined
null
```

**Checking for both `undefined` and `null` together (common real-world pattern):**
```javascript
let userInput;          // undefined — engine default
let deletedValue = null; // null — developer intentional

console.log(userInput == null);     // true  — loose equality treats both as "empty"
console.log(deletedValue == null);  // true
console.log(userInput === null);    // false — strict equality tells them apart
console.log(userInput === undefined); // true
```
**Output:**
```
true
true
false
true
```
This is why `== null` is a common (if slightly controversial) shortcut backend developers use to catch "this value is either undefined or null" in one check, instead of writing two separate `=== undefined` and `=== null` conditions.

---

## Important Keywords — Quick Reference

- **`var`** — Function-scoped, hoisted with `undefined`, allows re-declaration (bug-prone, avoided in modern code).
- **`let`** — Block-scoped, hoisted but uninitialized (TDZ), allows re-assignment, forbids re-declaration in the same scope.
- **`const`** — Block-scoped, TDZ on hoist, forbids re-assignment of the binding, but allows mutation of an object/array's contents.
- **Primitive types** — `Number`, `String`, `Boolean`, `Undefined`, `Null`, `Symbol`, `BigInt` — stored on the Stack, immutable, copied by value.
- **Reference types** — `Object`, `Array`, `Function` — stored on the Heap, mutable, copied by reference (pointer).
- **Dynamic Typing** — Types are determined at runtime based on the assigned value, and can change over a variable's lifetime.
- **Temporal Dead Zone (TDZ)** — The gap between scope entry and the actual declaration line for `let`/`const`, where access throws `ReferenceError`.
- **Uninitialized** — Memory exists, no value bound yet (TDZ state).
- **Undefined** — Engine-assigned placeholder meaning "declared, no value given."
- **Null** — Developer-assigned placeholder meaning "intentionally empty."
- **First-class function** — A function that can be stored in a variable, passed as an argument, and returned from another function, just like any other value.
- **`typeof null === "object"`** — A long-standing historical bug in JS, kept for backward compatibility.
