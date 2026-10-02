# PHASE 1 — C++ FUNDAMENTALS

## 1.1 Basic Program Structure
* **`main()` Function:** Every C++ program must have exactly one `main()` function. This is where the operating system transfers control to your program.
* **Statements vs. Expressions:** 
  * An *expression* evaluates to a value (e.g., `5 + 3`). 
  * A *statement* is a complete instruction ending with a semicolon `;` (e.g., `int x = 5 + 3;`).
* **Blocks:** Enclosed by `{ }`, blocks define a localized scope where variables live and die.
* **Namespaces:** A mechanism to prevent name collisions in large projects. For example, `std::` is the standard namespace. We avoid `using namespace std;` in professional code to prevent polluting our naming scope.
* **Headers vs. Source Files:** 
  * Header files (`.h` or `.hpp`) contain *declarations* (telling the compiler "this thing exists").
  * Source files (`.cpp`) contain *definitions* (the actual implementation logic).

## 1.2 Variables & Constants
* **Declaration vs. Definition vs. Initialization:**
  * *Declaration:* `extern int x;` (Promises the compiler `x` exists somewhere).
  * *Definition:* `int x;` (Actually allocates memory for `x`).
  * *Initialization:* `int x = 5;` (Allocates memory and immediately assigns a value).
* **Scope & Lifetime:** Scope is *where* a variable is visible in code; lifetime is *how long* the variable exists in memory before being destroyed.
* **Constants:**
  * `const`: A value that cannot be modified after initialization (evaluated at runtime or compile-time).
  * `constexpr`: A strict compile-time constant. The compiler calculates the value before the program ever runs, leading to zero runtime cost.
  * `consteval`: Introduced in C++20, forces a function to *only* execute at compile time.
  * `constinit`: Ensures a variable has static initialization, preventing the "static initialization order fiasco".

## 1.3 Data Types
* **Fundamental Types:** `bool` (true/false), `char` (1 byte character), integer types (`int`, `long`, `short`), floating-point types (`float`, `double`), and `void` (representing "nothing").
* **Signed vs. Unsigned:** `int` can be negative or positive. `unsigned int` can only be zero or positive, effectively doubling its maximum positive value.
* **Fixed-width Integers:** Professional C++ prefers types like `int32_t` or `uint64_t` (from `<cstdint>`) over standard `int` because standard `int` size can vary between different OS architectures.
* **Integer Overflow:** If you add 1 to the maximum possible value of a signed integer, it wraps around to the lowest negative number, causing Undefined Behavior.

## 1.4 Literals
* **Types of Literals:** Integer (`42`), Floating-point (`3.14f`), Character (`'A'`), String (`"Hello"`), and Boolean (`true`/`false`).
* **Number Bases:** You can write numbers in Hexadecimal (`0xFF`), Octal (`077`), or Binary (`0b1010`).
* **User-Defined Literals:** C++ allows you to create custom suffixes. For example, you can code `10s` to natively represent 10 seconds in the `<chrono>` library.

## 1.5 Operators
* **Standard Operators:** Arithmetic (`+`, `-`, `*`, `/`), Relational (`==`, `!=`, `<`), Logical (`&&`, `||`, `!`), and Assignment (`=`, `+=`).
* **Bitwise & Shift Operators:** Manipulate data at the binary level (`&`, `|`, `^`, `<<`, `>>`). Highly used in systems programming.
* **Memory/Type Operators:** 
  * `sizeof`: Returns the memory size of a type in bytes.
  * `alignof`: Returns the alignment boundary of a type.
  * `&` (Address-of): Retrieves the physical RAM address of a variable.
  * `*` (Dereference): Accesses the value stored at a specific memory address.
* **Scope Resolution (`::`):** Accesses elements inside a specific namespace or class.

## 1.6 Type Conversion
* **Implicit Conversion:** The compiler does it automatically. Beware of *narrowing conversions* (e.g., implicitly converting `double` to `int` truncates the decimal, losing data).
* **Explicit Conversion (Casts):**
  * **C-style casts:** `(int)3.14`. *Avoid this in professional C++* because it is unsafe and hard to search for in a codebase.
  * **C++ style casts:** 
    * `static_cast`: Safe, standard conversions (e.g., float to int).
    * `const_cast`: Removes the `const` qualifier from a variable (use with extreme caution).
    * `reinterpret_cast`: Brutally forces the compiler to treat a block of memory as a completely different type. Used in very low-level code.
    * `dynamic_cast`: Used for safe downcasting in Object-Oriented inheritance hierarchies (checked at runtime).

## 1.7 Input/Output
* **Streams:** C++ I/O uses a "stream" abstraction. Data flows into (`>>`) or out of (`<<`) your program.
* **Standard Streams:** 
  * `std::cin`: Standard input (usually keyboard).
  * `std::cout`: Standard output (usually console screen).
  * `std::cerr`: Standard error stream. This is *unbuffered*, meaning errors print immediately to the screen without waiting, which is critical during a program crash.
  * `std::clog`: Standard logging stream.
* **Manipulators:** Tools like `std::endl` (which inserts a newline AND flushes the buffer) or `std::setprecision` to format how data looks.

---

## Phase 1 Architecture Tree Diagrams

### 1. Intra-Phase Tree (Concept Hierarchies within Phase 1)
    Phase 1 — C++ Fundamentals
    ├── 1.1 Program Structure
    │   ├── main() Entry Point
    │   ├── Namespaces & Scope Blocks
    │   └── Header (.h) vs Source (.cpp)
    ├── 1.2 Variables & Constants
    │   ├── Declaration vs Definition
    │   └── Const Evaluation (const, constexpr, consteval)
    ├── 1.3 & 1.4 Types and Literals
    │   ├── Fundamental Types (int, float, char, bool)
    │   ├── Fixed-Width (int32_t) & Memory Sizes
    │   └── Base Literals (Hex, Binary, Octal)
    ├── 1.5 Operators
    │   ├── Math, Logic, Bitwise
    │   └── Memory Ops (sizeof, alignof, &, *)
    ├── 1.6 Type Conversions
    │   ├── Implicit vs Narrowing
    │   └── Modern Casts (static, const, reinterpret, dynamic)
    └── 1.7 I/O Streams
        ├── cin, cout, cerr (unbuffered)
        └── Stream Manipulators

### 2. Inter-Phase Bridge Tree (Connecting Phase 1 to the Rest of the Course)
    Phase 1 (Syntax, Types, Memory Basics)
     ├── Powers Logic ────> Phase 2 (Control Flow: if/else, loops)
     ├── Builds Actions ──> Phase 3 (Functions: wrapping basic statements)
     ├── Exposes Memory ──> Phase 5 (Pointers & References: using &, *)
     └── Demands Safety ──> Phase 8 (OOP) & Phase 10 (Exceptions: handling casting errors)

---

## FOLLOW-UP Q&A

**Q: Example of bitwise logic operators (`&`, `^`, `<<`, `>>`, `|`) with real binary logic?**

**A:** Assuming `a = 5` (`0101`) and `b = 3` (`0011`):
*   **Bitwise AND (`&`)**: `0101 & 0011` = `0001` (Result: 1). True only when both bits are 1.
*   **Bitwise OR (`|`)**: `0101 | 0011` = `0111` (Result: 7). True when at least one bit is 1.
*   **Bitwise XOR (`^`)**: `0101 ^ 0011` = `0110` (Result: 6). True when bits are different.
*   **Left Shift (`<< n`)**: `x << n` multiplies the number by 2^n. E.g., `5 << 1` shifts left and inserts 0 on the right (`1010`), resulting in 10. 
*   **Right Shift (`>> n`)**: `x >> n` divides the number by 2^n (integer division). E.g., `5 >> 1` shifts right and drops the rightmost bit (`0010`), resulting in 2.

**Q: How do I define a global variable for a modulo operation like 10^9 or 10^11 in different ways?**

**A:** There are 3 main ways, but remember that 10^9 fits inside a standard `int`, while 10^11 exceeds the maximum limit of a 32-bit `int` (which is approx 2*10^9) and **must** be stored in a `long long`.

1. **`constexpr` (Best & Modern Approach):** Fixes the value at compile-time for zero runtime overhead.
   `constexpr int MOD = 1e9 + 7;` 
   `constexpr long long MOD2 = 1e11;`
   *(Pro-tip: C++14 allows digit separators for readability: `constexpr int MOD = 1'000'000'007;`)*

2. **`const` keyword (Standard Approach):** Creates a read-only variable.
   `const int MOD = 1e9 + 7;`

3. **`#define` Macro (Old C-Style - Avoid):** A preprocessor directive that blindly replaces text before compilation. It lacks type safety.
   `#define MOD 1000000007`