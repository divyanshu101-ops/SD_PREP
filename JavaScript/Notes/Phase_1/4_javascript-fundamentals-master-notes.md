# JavaScript Fundamentals — Master Notes
### Execution Model, Memory, Variables, Data Types & Coercion

---

## PART 1 — HOW JAVASCRIPT ACTUALLY RUNS

### 1.1 Where JS Runs
JavaScript itself (ECMAScript) is just a language spec. It needs a **runtime environment** to execute:
- **Browser** → engine = V8 (Chrome), SpiderMonkey (Firefox), JavaScriptCore (Safari). Adds `window`, `document`, DOM APIs.
- **Node.js** → embeds V8 outside the browser. Adds `require`, `fs`, `process`, `global`, no DOM.

The **engine** only understands core JS (loops, functions, objects, math). Everything else (`document.querySelector`, `fs.readFile`) is a **host API** injected by the environment, not part of the language itself.

### 1.2 What Happens When You Run `node filename.js`
1. Node reads the file as raw text.
2. V8 **parses** it → checks syntax → builds an **AST (Abstract Syntax Tree)**.
3. V8's interpreter **Ignition** converts AST → bytecode → starts executing.
4. Before running any line, V8 does the **Memory Creation Phase** (hoisting — see Part 3).
5. Frequently-called ("hot") functions get compiled to optimized machine code by **TurboFan** (JIT compiler).
6. Node also spins up `libuv` (handles async I/O) and the **Event Loop** in the background.
7. Process exits once the call stack is empty **and** no pending timers/callbacks remain.

### 1.3 What Happens in Browser/V8 Console
- Same pipeline: Parse → AST → Bytecode → Execute — but one statement at a time.
- Global Execution Context is tied to `window` (`this` at top level = `window`), not `global`/`module` like Node.
- Variables declared in the console persist across commands in that session (shared context with the page).

---

## PART 2 — EXECUTION CONTEXT, CALL STACK & MEMORY ARCHITECTURE

### 2.1 Execution Context
An **Execution Context** is the environment created to evaluate and run code. A **Global Execution Context (GEC)** is created automatically the moment any JS program starts — even before your code runs.

Every execution context (global or function-level) goes through **two phases**:

**Phase 1 — Memory Creation Phase (Hoisting):**
- Engine scans the code top-to-bottom *before* executing anything.
- Allocates memory for every variable/function declaration it finds.
- (Exact rules per keyword covered in Part 3.)

**Phase 2 — Code Execution Phase:**
- Engine runs line-by-line, assigns real values, executes function calls.
- Each function call creates a **new Execution Context**, pushed onto the Call Stack.

### 2.2 The Call Stack
- JS is **single-threaded** → exactly one Call Stack.
- **LIFO (Last In, First Out)**: last function pushed is the first one popped.
- Flow: GEC pushed first → each function call pushes a new context on top → function returns/ends → context popped, memory cleared.
- Deep uncontrolled recursion → **"Maximum call stack size exceeded."**

```javascript
function greet() {
  return sayHi();
}
function sayHi() {
  return "Hi!";
}
console.log(greet());
```
**Stack order:** GEC → `greet()` → `sayHi()` → `sayHi()` pops → `greet()` pops → GEC.
**Output:**
```
Hi!
```

### 2.3 Memory Architecture — Stack vs Heap

| | **Call Stack** | **Heap** |
|---|---|---|
| Size | Fixed, small, fast | Large, dynamic |
| Stores | Primitives, execution contexts | Objects, Arrays, Functions |
| Copy behavior | **By value** — real copy made | **By reference** — pointer copied, same data shared |

**Primitive → copy by value:**
```javascript
let a = 10;
let b = a;
b = 20;
console.log(a); // 10 — untouched, independent copy
console.log(b); // 20
```
**Output:**
```
10
20
```

**Reference type → copy by reference (pointer):**
```javascript
let obj1 = { name: "Divyanshu" };
let obj2 = obj1;          // obj2 gets the SAME heap address, not a new object
obj2.name = "Backend";
console.log(obj1.name);   // "Backend" — obj1 changed too!
```
**Output:**
```
Backend
```
*(Note: "stack/heap" is a simplification used for teaching. The spec only guarantees primitives behave by-value and objects by-reference — where V8 actually stores things internally is an implementation detail. But this mental model is what you should code with.)*

---

## PART 3 — VARIABLE DECLARATION KEYWORDS

### 3.1 `var` — Function Scoped
- Hoisted, memory pre-filled with `undefined`.
- Ignores `{ }` block boundaries — only confined by nearest function (or global).
- **Re-declaration allowed** → silent overwrite, source of hidden bugs → avoided in modern backend code.

```javascript
console.log(x); // undefined, not an error
var x = 10;
console.log(x); // 10

function test() {
  if (true) {
    var y = 5;
  }
  console.log(y); // 5 — leaks out of the if-block
}
test();

var age = 25;
var age = 30;    // no error, silently overwrites
console.log(age); // 30
```
**Output:**
```
undefined
10
5
30
```

### 3.2 `let` — Block Scoped
- Hoisted but **not initialized** → sits in the **Temporal Dead Zone (TDZ)** until its declaration line runs.
- Respects `{ }` boundaries.
- Re-assignment allowed, **re-declaration in the same scope forbidden**.

```javascript
console.log(z); // ReferenceError
let z = 5;

if (true) {
  let b = 10;
}
console.log(b); // ReferenceError — b doesn't leak

let score = 50;
score = 80;       // fine — reassignment
console.log(score);

let score = 100;  // SyntaxError — redeclaration
```
**Output:**
```
Uncaught ReferenceError: Cannot access 'z' before initialization
Uncaught ReferenceError: b is not defined
80
Uncaught SyntaxError: Identifier 'score' has already been declared
```

**TDZ + `typeof` gotcha:** normally `typeof undeclaredVar` safely returns `"undefined"`, but inside a TDZ it still throws, because the engine already knows the variable exists in that scope (hoisted), just not initialized.
```javascript
{
  console.log(typeof c); // ReferenceError, not "undefined"
  let c = 5;
}
```

### 3.3 `const` — Block Scoped, Immutable Binding
- Same TDZ behavior as `let`.
- **Reassignment of the binding forbidden.**
- **Contents of an object/array CAN still be mutated** — `const` locks the variable to one heap address, not the data inside it.

```javascript
const PI = 3.14;
PI = 3.14159; // TypeError

const user = { name: "Divyanshu", age: 22 };
user.age = 23;           // allowed — mutating a property
console.log(user);
user = { name: "New" };  // TypeError — reassigning the variable itself

const numbers = [1, 2, 3];
numbers.push(4);         // allowed — array mutation
console.log(numbers);
numbers = [5, 6, 7];     // TypeError
```
**Output:**
```
Uncaught TypeError: Assignment to constant variable.
{ name: 'Divyanshu', age: 23 }
Uncaught TypeError: Assignment to constant variable.
[ 1, 2, 3, 4 ]
Uncaught TypeError: Assignment to constant variable.
```

---

## PART 4 — DATA TYPES

### 4.1 Primitives (Stack, Immutable, Copy by Value)

```javascript
let num = 25.5;              // Number — ints + decimals, 64-bit float
let str = "Hello";            // String
let flag = true;              // Boolean
let notAssigned;               // Undefined
let empty = null;             // Null
let id = Symbol("id");         // Symbol — unique, immutable
let big = 9007199254740993n;   // BigInt — beyond Number.MAX_SAFE_INTEGER
```

**"Immutable" means every operation returns a NEW value, original is untouched:**
```javascript
let s = "hello";
s.toUpperCase();
console.log(s);              // "hello" — original untouched
console.log(s.toUpperCase()); // "HELLO" — new string returned
```

**`typeof null` bug:**
```javascript
console.log(typeof null); // "object" — a bug from 1995, kept for backward compatibility
```

**Number precision limit vs BigInt:**
```javascript
console.log(Number.MAX_SAFE_INTEGER);      // 9007199254740991
console.log(Number.MAX_SAFE_INTEGER + 2);  // 9007199254740992 — WRONG, precision lost
let safe = 9007199254740993n;              // BigInt — exact
console.log(safe);                          // 9007199254740993n
```

### 4.2 Reference Types (Heap, Mutable, Copy by Reference)

```javascript
let person = { name: "Divyanshu", role: "Backend Dev" }; // Object
let skills = ["Node.js", "System Design", "AI"];          // Array (technically an Object)
function greet() { return "Hi"; }                          // Function
```

**Functions are "first-class citizens"** — can be stored, passed, returned like any value:
```javascript
function sayHi() { return "Hi!"; }
let ref = sayHi;                       // stored in a variable
console.log(ref());

function callTwice(fn) { return fn() + " " + fn(); } // passed as argument
console.log(callTwice(sayHi));

function makeGreeter() {               // returned from another function
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

---

## PART 5 — MEMORY STATES: Uninitialized vs Undefined vs Null

| State | Applies to | Cause | Accessing it |
|---|---|---|---|
| **Uninitialized** | `let`, `const` | Hoisted, no value bound yet (TDZ) | Throws `ReferenceError` |
| **Undefined** | `var`, any variable declared without a value | Engine auto-assigns placeholder | Returns `undefined` |
| **Null** | Any variable | Developer explicitly writes `= null` | Returns `null` |

```javascript
console.log(a); // ReferenceError — uninitialized (TDZ)
let a = 1;

console.log(b); // undefined — engine default
var b = 2;

let c = null;    // intentional emptiness
console.log(c);
```
**Output:**
```
Uncaught ReferenceError: Cannot access 'a' before initialization
undefined
null
```

**Practical pattern — checking for "empty" (undefined OR null) in one shot:**
```javascript
let userInput;           // undefined
let deletedValue = null; // null

console.log(userInput == null);      // true  — == treats undefined and null as equal
console.log(deletedValue == null);   // true
console.log(userInput === null);     // false — strict, different types
console.log(userInput === undefined);// true
```
**Output:**
```
true
true
false
true
```

---

## PART 6 — DYNAMIC TYPING [Interview Important]

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
No type is declared upfront — V8 checks the value at the moment of assignment and labels the variable's *current* type. The same variable can hold different types across its life — the opposite of a statically typed language like C++, where `int x = 5;` locks `x` to `int` forever.

---

## PART 7 — TYPE COERCION (Automatic Type Conversion) [Interview Important]

JS is loosely-typed — mixing types in an operation triggers **implicit coercion**, where the engine auto-converts one type to another behind the scenes.

```javascript
console.log(5 + "5");    // "55" — Number → String, then concatenated
console.log("5" - 2);    // 3   — "-" forces numeric coercion, string → number
console.log(true + 1);   // 2   — true → 1
```
**Output:**
```
55
3
2
```

**Important distinction: `+` vs other operators**
- `+` → if **either side is a string**, it does **concatenation** (string coercion).
- `-`, `*`, `/` → always force **numeric coercion**, even on strings.

```javascript
console.log("5" + 2);    // "52" — concatenation
console.log("5" * 2);    // 10   — numeric coercion
console.log("abc" - 2);  // NaN  — "abc" can't become a number
```
**Output:**
```
52
10
NaN
```

---

## PART 8 — `==` (Loose Equality) vs `===` (Strict Equality)

- **`==`**: compares after **coercing** types to match, if they differ.
- **`===`**: compares **value AND type**, no coercion — the recommended default in real code.

```javascript
console.log(5 == "5");   // true  — "5" coerced to 5, then compared
console.log(5 === "5");  // false — Number vs String, no coercion, straight false
```
**Output:**
```
true
false
```

**Edge cases worth memorizing for interviews:**

```javascript
console.log(null == undefined);  // true  — special-cased by spec, only these two are == to each other
console.log(null === undefined); // false — different types
console.log(null == 0);          // false — null does NOT coerce to any number, only to undefined

console.log(NaN === NaN);        // false! — NaN is never equal to itself, by IEEE 754 spec
console.log(Number.isNaN(NaN));  // true  — correct way to check for NaN
```
**Output:**
```
true
false
false
false
true
```

---

## PART 9 — JS RUNTIME BEHAVIOR vs COMPILED LANGUAGES (C++)

- **C++**: separate compile step. A **type error anywhere** stops compilation — **zero output**, ever, even for correct lines above the error.
- **JavaScript**: parses the **whole file first** (so a **syntax error anywhere** still blocks the entire program from running — same as C++ in that specific case) — but **type errors are only caught when that exact line actually executes**, since JS is dynamically typed.

```javascript
let x = 5;
console.log("Start");
x = x + "hello";   // JS coerces instead of throwing
console.log(x);
console.log("End");
```
**Output:**
```
Start
5hello
End
```
Compare to something that genuinely throws at runtime:
```javascript
let y = 5;
console.log("Start");
y();                // TypeError — y is not a function
console.log("End");
```
**Output:**
```
Start
Uncaught TypeError: y is not a function
End never prints — crash happens mid-execution
```

---

## QUICK-REFERENCE GLOSSARY

- **Execution Context** — environment where code is evaluated/run; created for global scope and every function call.
- **Global Execution Context (GEC)** — the base context created once when the program starts.
- **Call Stack** — single LIFO structure tracking active execution contexts.
- **Hoisting** — declarations processed and given memory before code runs.
- **Memory Creation Phase** — first pass of an execution context; hoisting happens here.
- **Code Execution Phase** — second pass; real values assigned, code actually runs.
- **Temporal Dead Zone (TDZ)** — gap between scope entry and the declaration line for `let`/`const`; accessing here throws.
- **AST** — tree structure of parsed code, built before bytecode generation.
- **Ignition** — V8's interpreter (AST → bytecode → execution).
- **TurboFan** — V8's JIT compiler for optimizing "hot" code into machine code.
- **Stack memory** — fast, fixed-size; stores primitives + execution context data, by value.
- **Heap memory** — large, dynamic; stores objects/arrays/functions, by reference (pointer).
- **Primitive types** — `Number`, `String`, `Boolean`, `Undefined`, `Null`, `Symbol`, `BigInt` — immutable, copy by value.
- **Reference types** — `Object`, `Array`, `Function` — mutable, copy by reference.
- **First-class function** — can be stored in a variable, passed as an argument, returned from a function.
- **Dynamic typing** — type is determined at runtime from the assigned value, and can change over time.
- **Type Coercion** — engine's automatic conversion between types during an operation.
- **`==`** — loose equality, coerces types before comparing.
- **`===`** — strict equality, compares value + type, no coercion (preferred in real code).
- **`typeof null === "object"`** — historical engine bug from 1995, kept for backward compatibility.
- **`NaN !== NaN`** — NaN is never equal to itself; use `Number.isNaN()` to check.
