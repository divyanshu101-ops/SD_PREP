# JavaScript Mastery Syllabus: From Basics to Advanced Backend

## Phase 1: Engine Mechanics & Core Syntax
- **Execution Context & The Call Stack:** How the JavaScript engine parses and runs code line-by-line. [Interview Important]
- **Variables, Scope, and Memory:** `let`, `const`, `var`, hoisting, and the Temporal Dead Zone (TDZ). [Interview Important]
- **Data Types & Type Coercion:** Primitives vs. Reference types, memory allocation (Stack vs. Heap), and strict equality.
- **Functions as First-Class Citizens:** Declarations, expressions, arrow functions, and Immediately Invoked Function Expressions (IIFEs). Passing functions as synchronous callbacks.

## Phase 2: Data Manipulation & ES6+ Patterns
- **Arrays & Higher-Order Methods:** Mastering `map`, `filter`, `reduce`, `find`, and `sort` for robust data transformation. [Interview Important]
- **Building Custom Higher-Order Functions:** Recreating array methods to deeply understand synchronous callbacks. [Interview Important]
- **Object Architecture:** Property descriptors, dynamic keys, and the mechanics of deep vs. shallow copying.
- **Modern Unpacking:** Object/Array destructuring, rest parameters, and the spread operator.

## Phase 3: The "Hard Parts" of JavaScript
- **Lexical Environment & Closures:** Understanding data privacy, state retention, and memoization. [Interview Important]
- **The `this` Context:** Implicit vs. explicit binding (`call`, `apply`, `bind`), and how arrow functions alter context. [Interview Important]
- **Prototypes & Inheritance:** The prototype chain, `__proto__`, and prototypal delegation before ES6 classes. [Interview Important]

## Phase 4: Asynchronous I/O & The Event Loop (Backend Core)
- **The Event Loop Architecture:** Call stack, background APIs (C++ threads), Task Queue, and Microtask Queue. [Interview Important]
- **Asynchronous Callbacks:** Inversion of control and avoiding callback hell (Pyramid of Doom). [Interview Important]
- **Promises:** State machines (pending, fulfilled, rejected), chaining, and concurrency (`Promise.all`, `Promise.allSettled`). [Interview Important]
- **Async / Await:** Writing synchronous-looking asynchronous code, parallel execution strategies, and rigorous `try/catch` error handling. [Interview Important]

## Phase 5: Architecture, Errors, & Advanced Patterns
- **Modularity:** CommonJS (`require`/`module.exports`) vs. ES Modules (`import`/`export`).
- **Error Management:** Extending the native `Error` class, capturing stack traces, and handling unhandled promise rejections. [Interview Important]
- **OOP in JS:** ES6 Classes, encapsulation, inheritance, and static methods.
- **Garbage Collection:** Mark-and-sweep algorithms, memory leaks, and using `WeakMap`/`WeakSet`.

## Phase 6: Bridging to the Server
- **V8 Engine Basics:** JIT (Just-In-Time) compilation and how to write predictable, optimized code.
- **Node.js Fundamentals:** Event Emitters, Buffers, and Streams (handling large data without crashing memory). [Interview Important]