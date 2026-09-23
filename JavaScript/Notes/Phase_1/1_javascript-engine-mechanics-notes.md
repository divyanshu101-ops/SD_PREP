# JavaScript Engine Mechanics & Core Syntax — Complete Notes

## 1. History: Why JavaScript Was Created

- JavaScript was created in **1995** by **Brendan Eich**, an engineer at **Netscape Communications**.
- It was built in just **10 days** to give Netscape Navigator a lightweight scripting language that could make static HTML pages interactive (form validation, dynamic content, simple animations) without needing a full page reload.
- Originally named **Mocha**, then **LiveScript**, and finally renamed **JavaScript** as a marketing move to ride on the popularity of Java at the time — despite the two languages having almost nothing in common technically.
- In 1997, JavaScript was standardized by **ECMA International** under the name **ECMAScript (ES)**. This is why you hear terms like ES5, ES6 (ES2015), ES2020, etc. — these are versions of the official specification that all JS engines must follow.
- Originally JavaScript could only run **inside a web browser**. It could not read files, access a database, or create a server. That changed in **2009** when **Ryan Dahl** created **Node.js**, which took Chrome's V8 engine and embedded it inside a standalone C++ program — allowing JavaScript to run outside the browser, on a server, with access to the file system, networking, and OS-level APIs.

---

## 2. Where JavaScript Can Run

JavaScript itself is just a language specification (ECMAScript). It needs a **runtime environment** to actually execute. The two major environments are:

| Environment | Engine Used | What It Adds |
|---|---|---|
| **Browser** (Chrome, Edge, Brave) | V8 | `window`, `document`, DOM APIs, `fetch`, `localStorage` |
| **Node.js** (server/terminal) | V8 (embedded) | `require`/`import` modules, `fs`, `http`, `process`, `Buffer` |
| Firefox | SpiderMonkey | Browser APIs (different engine, same ECMAScript spec) |
| Safari | JavaScriptCore | Browser APIs |

**Key idea:** The *engine* (V8, SpiderMonkey, etc.) only understands core JavaScript — loops, functions, objects, math, closures. Anything else — like `document.querySelector` in a browser, or `fs.readFile` in Node — is **not part of JavaScript itself**. It's an API injected into the environment by the *host* (browser or Node), which the engine can then call into.

---

## 3. What Happens When You Run `node filename.js`

Step by step:

1. **You type the command** in the terminal: `node filename.js`.
2. Node.js **reads the file** from disk as raw text.
3. Node hands this text to the **V8 engine**, which first checks it for syntax errors (**parsing** stage) and converts it into an **AST (Abstract Syntax Tree)** — a tree-like structural representation of your code.
4. V8's interpreter, called **Ignition**, converts the AST into **bytecode** and begins executing it line by line.
5. Before execution actually starts, V8 performs the **Memory Creation Phase** (hoisting) — see Section 5 below.
6. As code runs, if V8 notices a function is being called repeatedly ("hot" code), its JIT (Just-In-Time) compiler, **TurboFan**, compiles that function into highly optimized **machine code** for speed.
7. Node also sets up its own internal machinery in the background — the **Event Loop**, **libuv** (a C++ library handling async I/O like file reads, timers, network calls), and global Node-specific objects (`process`, `module`, `require`, `__dirname`, etc.) that don't exist in a browser.
8. Execution finishes when the call stack is empty **and** there are no pending timers/callbacks/I/O left in the event loop — at that point the Node process exits automatically.

---

## 4. What Happens When You Run JS in the Browser (V8/DevTools Console)

1. You open **DevTools → Console** in Chrome (which uses V8 directly, same engine as Node).
2. You type a line of JS and hit Enter.
3. Unlike a `.js` file, the console evaluates code **one statement at a time**, but it still goes through the same core pipeline: **Parse → AST → Bytecode (Ignition) → Execution**.
4. The **Global Execution Context** here is tied to the `window` object (not `global`/`module` like in Node) — so `this` at the top level refers to `window`.
5. You get access to **browser-only APIs**: `document`, `window`, `alert`, `localStorage`, `fetch`, DOM manipulation methods — none of which exist in plain Node.js.
6. Each statement you run in the console shares the **same execution context/memory** as the page — variables you declare persist across console commands in that session, similar to how a `<script>` tag on the page would behave.

---

## 5. Execution Context & The Call Stack

An **Execution Context** is the environment created by the engine to evaluate and run code. The moment any JS file starts running, a **Global Execution Context (GEC)** is created automatically — even before your code does anything.

### Two Phases of Every Execution Context

**Phase 1 — Memory Creation Phase (a.k.a. "Hoisting")**
- The engine scans the code top to bottom *before* running anything.
- It allocates memory for every variable and function it finds.
- `var` variables → memory slot created and pre-filled with the placeholder value **`undefined`**.
- `let` and `const` variables → memory slot is reserved but **not initialized** — they sit in the **Temporal Dead Zone (TDZ)** until their declaration line is actually executed. Accessing them before that throws a `ReferenceError`, not `undefined`.
- Function **declarations** (`function foo(){}`) → the **entire function body** is stored in memory immediately, which is why you can call a declared function *before* the line it's written on.
- Function **expressions** and **arrow functions** assigned to variables behave like the variable rules above (`var`/`let`/`const`), not like full hoisting.

**Phase 2 — Code Execution Phase**
- The engine runs through the code a second time, this time actually executing each line.
- Real values are assigned to variables (replacing the `undefined` placeholders).
- Function calls are made, and each call creates a **new Execution Context** for that function.

### The Call Stack
- JavaScript is **single-threaded** — it has exactly one Call Stack and can only do one thing at a time.
- The Call Stack is a **LIFO (Last In, First Out)** structure.
- Flow: Global Execution Context is pushed first → every function call pushes a new context on top → when a function finishes (returns or ends), its context is popped off and its memory is cleared.
- If functions call each other too deeply without stopping (e.g. infinite recursion), the stack overflows → **"Maximum call stack size exceeded"** error.

---

## 6. Memory Architecture: Stack vs Heap

The JS engine divides the memory it uses (inside your system's RAM) into two regions:

- **Call Stack (Stack Memory):** Fixed-size, fast, highly organized. Stores:
  - Primitive values: `string`, `number`, `boolean`, `undefined`, `null`, `symbol`, `bigint`
  - Execution contexts and function call information
  - Stored by **value** — copying a primitive copies the actual data.

- **Heap (Heap Memory):** Large, unstructured, dynamically sized. Stores:
  - Reference types: `Object`, `Array`, `Function`
  - The Stack only holds a **reference (memory address/pointer)** to where the actual object lives in the Heap.
  - Stored by **reference** — copying a reference type copies the *pointer*, not the data, meaning two variables can end up pointing to the same object in memory.

This distinction is exactly why in JS, mutating an object through one variable affects another variable that "points" to it, while reassigning a primitive doesn't affect other copies.

---

## Important Keywords — Quick Reference

- **Execution Context** — The environment in which JS code is evaluated and run; created for global code and for every function call.
- **Global Execution Context (GEC)** — The default/base execution context created once when a JS program starts running.
- **Call Stack** — The single, LIFO structure that tracks which execution context is currently running.
- **Hoisting** — The behavior where variable and function declarations are processed and given memory *before* the code actually runs.
- **Memory Creation Phase** — The first pass of an execution context, where memory is allocated for variables/functions (hoisting happens here).
- **Code Execution Phase** — The second pass, where actual code runs line-by-line and values are assigned.
- **Temporal Dead Zone (TDZ)** — The period between entering scope and the actual declaration line where `let`/`const` variables exist but cannot be accessed.
- **AST (Abstract Syntax Tree)** — A tree representation of your source code that the engine builds after parsing, used before generating bytecode.
- **Ignition** — V8's interpreter; converts AST into bytecode and starts executing it.
- **TurboFan** — V8's JIT (Just-In-Time) optimizing compiler; converts "hot" (frequently run) bytecode into fast machine code.
- **Engine** — The program that actually parses and executes JS (e.g., V8, SpiderMonkey, JavaScriptCore). Understands only core language features.
- **Runtime Environment** — The engine plus extra host-provided APIs (browser or Node) that let JS interact with the outside world (DOM, files, network).
- **libuv** — The C++ library Node uses to handle asynchronous I/O (file system, network, timers) and the event loop.
- **Event Loop** — The mechanism that allows single-threaded JS to handle async operations by managing the call stack, callback queues, and pending I/O.
- **Stack Memory** — Fast, fixed-size memory that stores primitives and execution context data, by value.
- **Heap Memory** — Large, dynamic memory that stores objects/arrays/functions, by reference.
- **Primitive Types** — `string`, `number`, `boolean`, `undefined`, `null`, `symbol`, `bigint` — immutable, stored on the stack.
- **Reference Types** — `Object`, `Array`, `Function` — mutable, stored on the heap, accessed via a pointer on the stack.
