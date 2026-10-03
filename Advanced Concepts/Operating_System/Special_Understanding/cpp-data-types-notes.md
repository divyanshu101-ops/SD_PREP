# C++ Data Types, Type Modifiers, Size and Range (with 32-bit vs 64-bit)

*These notes explain how C++ data types connect to RAM, the OS and the CPU. Section 8 links everything to the 32-bit vs 64-bit notes. Read it once fully, then use the revision table.*

---

## 1. Why data types exist: the memory and OS view

### RAM as a warehouse

Think of RAM as a huge warehouse with trillions of small storage boxes called **bytes**. Every box has a unique numeric **address**, so the OS and CPU can find it.

When your C++ program runs, the OS gives your process a private part of this warehouse.

But RAM is **blind and unformatted**. It does not know what is stored inside the boxes. It only holds raw electrical states that we read as bits (0s and 1s).

### The same bits can mean different things

Look at this exact 8-bit pattern sitting in RAM:

```
01000001
```

What is it?

- The integer **65**?
- The ASCII character **'A'**?
- A small piece of a floating-point number?

In RAM, all three look **identical**. Only the data type tells us how to read the bits.

```cpp
#include <iostream>
using namespace std;

int main() {
    char c = 65;                       // bits: 01000001
    cout << c << endl;                 // prints: A   (read as a character)
    cout << (int)c << endl;            // prints: 65  (read as a number)
}
```

Same bits, two different outputs. The type decides the meaning.

### What a data type tells the compiler

A **data type** is a contract (a blueprint) between you and the compiler. It defines three things:

1. **Size:** how many bytes in RAM the variable uses (1, 4, 8 bytes and so on).
2. **Interpretation:** how to decode the bits (signed integer, IEEE 754 floating-point number, a raw memory address and so on).
3. **Valid operations:** which CPU instructions are allowed. For example, you can multiply floats, but a bitwise shift on a float makes no sense, so the compiler gives an error.

---

## 2. Quick reminder: bits, bytes and registers

- 1 **bit** = one 0 or 1.
- 1 **byte** = 8 bits. A byte is the **smallest addressable unit** of memory.
- n bits give 2^n different patterns.

```
1 byte  = 8 bits   -> 2^8  = 256 patterns
2 bytes = 16 bits  -> 2^16 = 65,536 patterns
4 bytes = 32 bits  -> 2^32 = 4,294,967,296 patterns
8 bytes = 64 bits  -> 2^64 = 18,446,744,073,709,551,616 patterns
```

The size of a type decides how many different values it can store.

---

## 3. Fundamental built-in data types

The exact sizes can vary slightly with the CPU architecture (32-bit vs 64-bit) and the compiler. On modern 64-bit systems (x86_64 / ARM64) the sizes below are the standard ones.

### 3.1 `bool` (Boolean)

- **Size:** 1 byte (8 bits).
- **Why 1 byte?** A boolean needs only 1 bit (0 or 1), but the smallest addressable unit of memory is a byte. So a `bool` uses one full byte.
- **Storage:** `00000000` is `false`. Any non-zero value (typically `00000001`) is `true`.
- **Range:** `false` (0) or `true` (1).

```
false -> 00000000
true  -> 00000001
```

### 3.2 `char` (Character)

- **Size:** 1 byte (8 bits).
- **Storage:** text characters mapped through ASCII, or small integer values.
- **Range:**
  - `signed char`: -128 to 127
  - `unsigned char`: 0 to 255

```
'A' = 65 = 01000001
'a' = 97 = 01100001
```

### 3.3 `int` (Integer)

- **Size:** 4 bytes (32 bits) on standard modern architectures.
- **Storage:** **Two's complement**. This lets the CPU handle positive and negative numbers with the same binary addition circuit. The highest bit (bit 31) is the **sign bit**: 0 means positive, 1 means negative.
- **Range:** -2,147,483,648 to 2,147,483,647 (roughly -2 x 10^9 to +2 x 10^9).

```
int 5   -> 00000000 00000000 00000000 00000101
int -1  -> 11111111 11111111 11111111 11111111
```

### 3.4 `float` (single-precision floating point)

- **Size:** 4 bytes (32 bits).
- **Storage:** IEEE 754 standard. The 32 bits are split into three parts:
  - 1 bit for the **sign** (+ or -)
  - 8 bits for the **exponent** (the scaling factor)
  - 23 bits for the **mantissa / fraction** (the significant digits)
- **Range:** about +-1.2 x 10^-38 to +-3.4 x 10^38, with about 6 to 7 decimal digits of precision.

```
float 1.0:

sign  exponent   mantissa
0     01111111   00000000000000000000000      (= 0x3F800000)
1 bit 8 bits     23 bits
```

### 3.5 `double` (double-precision floating point)

- **Size:** 8 bytes (64 bits).
- **Storage:** also IEEE 754, with a bigger layout:
  - 1 bit for the **sign**
  - 11 bits for the **exponent**
  - 52 bits for the **mantissa**
- **Range:** about +-2.3 x 10^-308 to +-1.7 x 10^308, with about 15 to 17 decimal digits of precision.

### 3.6 `void`

- **Size:** 0 bytes. It is an **incomplete type**, so you cannot create a normal variable of type `void`.
- **Storage:** it does not allocate any memory. It means "no type" or "absence of value".
- **Used for:**
  - functions that return nothing (`void print();`)
  - generic pointers (`void*`)

---

## 4. Type modifiers

Type modifiers change the storage size, capacity or sign interpretation of a basic type. C++ has four main modifiers:

| Modifier | Meaning |
|---|---|
| `signed` | Can store positive and negative numbers. This is the default for integers. |
| `unsigned` | Stores only non-negative numbers. The range shifts so the positive capacity is doubled. |
| `short` | Reduces the size, typically 2 bytes. |
| `long` / `long long` | Increases the size, typically 4 or 8 bytes. |

### How ranges are calculated

If a type uses **n bits**:

```
Unsigned range:  0  to  2^n - 1
Signed range:    -2^(n-1)  to  2^(n-1) - 1     (two's complement)
```

**Worked examples:**

```
8 bits  unsigned: 0 to 2^8 - 1 = 0 to 255
8 bits  signed  : -2^7 to 2^7 - 1 = -128 to 127

16 bits unsigned: 0 to 65,535
16 bits signed  : -32,768 to 32,767

32 bits unsigned: 0 to 4,294,967,295
32 bits signed  : -2,147,483,648 to 2,147,483,647
```

### Why does signed lose half of the positive range?

One bit is used as the sign bit, so only n - 1 bits are left for the size of the number.

```
8-bit unsigned: 00000000 ... 11111111  = 0 ... 255
8-bit signed  : 10000000 ... 01111111  = -128 ... 127

Signed 8-bit examples (two's complement):
00000000 =    0
00000001 =    1
01111111 =  127   (biggest positive)
10000000 = -128   (smallest negative)
11111111 =   -1
```

The total number of patterns is the same (256). Signed just spends about half of them on negative numbers.

---

## 5. Comprehensive table of modifiers and sizes (standard 64-bit system)

| Data type / modifier | Size (bytes) | Bit width | Range |
|---|---|---|---|
| `short int` (or `short`) | 2 | 16 | -32,768 to 32,767 |
| `unsigned short int` | 2 | 16 | 0 to 65,535 |
| `int` | 4 | 32 | -2,147,483,648 to 2,147,483,647 |
| `unsigned int` | 4 | 32 | 0 to 4,294,967,295 |
| `long int` (or `long`) | 4 or 8 | 32 or 64 | Architecture dependent |
| `long long int` (or `long long`) | 8 | 64 | about -9 x 10^18 to about 9 x 10^18 |
| `unsigned long long` | 8 | 64 | 0 to about 1.8 x 10^19 |
| `float` | 4 | 32 | +-1.2 x 10^-38 to +-3.4 x 10^38 |
| `double` | 8 | 64 | +-2.3 x 10^-308 to +-1.7 x 10^308 |
| `long double` | 8, 12 or 16 | 64 to 128 | Extended-precision floating point |

Also remember from section 3:

| Type | Size | Range |
|---|---|---|
| `bool` | 1 byte | false (0) or true (1) |
| `char` / `signed char` | 1 byte | -128 to 127 |
| `unsigned char` | 1 byte | 0 to 255 |
| `void` | 0 (incomplete type) | no value |

---

## 6. Memory layout example

Here is how a few variables could sit in RAM (each box is 1 byte, addresses are only examples):

```cpp
bool  flag = true;
char  ch   = 'A';
int   num  = 5;
```

```
Address   Byte value   Belongs to
1000      00000001     flag  (1 byte)
1001      01000001     ch    (1 byte)
1002      00000101     num   (byte 1 of 4, little-endian: lowest byte first)
1003      00000000     num   (byte 2 of 4)
1004      00000000     num   (byte 3 of 4)
1005      00000000     num   (byte 4 of 4)
```

In real programs the compiler may add small gaps (padding) so that variables sit at convenient addresses. The idea stays the same: **the type decides how many bytes a variable occupies and how its bits are read.**

---

## 7. Check sizes yourself with `sizeof`

```cpp
#include <iostream>
using namespace std;

int main() {
    cout << "bool        : " << sizeof(bool)        << " byte(s)" << endl;
    cout << "char        : " << sizeof(char)        << " byte(s)" << endl;
    cout << "short       : " << sizeof(short)       << " byte(s)" << endl;
    cout << "int         : " << sizeof(int)         << " byte(s)" << endl;
    cout << "long        : " << sizeof(long)        << " byte(s)" << endl;
    cout << "long long   : " << sizeof(long long)   << " byte(s)" << endl;
    cout << "float       : " << sizeof(float)       << " byte(s)" << endl;
    cout << "double      : " << sizeof(double)      << " byte(s)" << endl;
    cout << "long double : " << sizeof(long double) << " byte(s)" << endl;
    cout << "int*        : " << sizeof(int*)        << " byte(s)" << endl;
}
```

Typical output on a 64-bit Linux or macOS machine:

```
bool        : 1 byte(s)
char        : 1 byte(s)
short       : 2 byte(s)
int         : 4 byte(s)
long        : 8 byte(s)
long long   : 8 byte(s)
float       : 4 byte(s)
double      : 8 byte(s)
long double : 16 byte(s)
int*        : 8 byte(s)
```

Your output may differ slightly by compiler and OS. That is why `sizeof` is the safe way to check.

To print the exact limits of a type:

```cpp
#include <iostream>
#include <limits>
using namespace std;

int main() {
    cout << "int min: " << numeric_limits<int>::min() << endl;   // -2147483648
    cout << "int max: " << numeric_limits<int>::max() << endl;   //  2147483647
    cout << "unsigned int max: " << numeric_limits<unsigned int>::max() << endl; // 4294967295
}
```

---

## 8. Connecting data types to 32-bit vs 64-bit

This is where your previous notes (registers and addresses) meet data types.

### 8.1 The big picture

- A **data type** decides how many bytes a variable uses and how to read its bits.
- A **32-bit or 64-bit CPU** decides how wide its registers are, and so how wide an address can be.
- A variable lives in RAM at an **address**. The CPU loads the variable into a **register** to work on it.

```
   RAM                          CPU
+-----------------+          +----------------------+
| address 4096    |  load    | register R1          |
| int num = 25    | -------> | 25  (32-bit value)   |
|                 |          |                      |
| address 8192    |  load    | register R2          |
| double d = 3.5  | -------> | 8-byte value         |
+-----------------+          +----------------------+
```

### 8.2 Which types change between 32-bit and 64-bit, and which do not

| Type | 32-bit system | 64-bit system | Changes? |
|---|---|---|---|
| `bool`, `char` | 1 byte | 1 byte | No |
| `short` | 2 bytes | 2 bytes | No |
| `int` | 4 bytes | 4 bytes | No (in practice) |
| `float` | 4 bytes | 4 bytes | No |
| `double` | 8 bytes | 8 bytes | No |
| `long long` | 8 bytes | 8 bytes | No |
| `long` | 4 bytes | 4 bytes on 64-bit Windows, 8 bytes on 64-bit Linux/macOS | **Yes, depends** |
| **Pointer** (`int*`, `void*`) | **4 bytes** | **8 bytes** | **Yes** |
| `size_t` (size and index type) | 4 bytes | 8 bytes | **Yes** |
| `long double` | often 12 bytes | 8 or 16 bytes (compiler dependent) | Yes, depends |

Key idea: **ordinary data types like `int` and `double` keep their size. Pointers (addresses) grow, because an address must fit in a register-wide value.**

This is why the document says `long` is "architecture dependent": it is the type that the OS and compiler resize.

### 8.3 Pointer size = address width

A pointer is just a variable that stores an address. Its size matches the address width of the CPU.

```cpp
int x = 10;
int* p = &x;

cout << sizeof(x) << endl;   // 4  (int is 4 bytes on both 32-bit and 64-bit)
cout << sizeof(p) << endl;   // 4 on 32-bit, 8 on 64-bit
```

- On a 32-bit system, an address has 32 bits, so there are 2^32 addresses (4 GB).
- On a 64-bit system, an address has 64 bits, so there are 2^64 addresses (in practice 48 bits are used, about 256 TB per process).

### 8.4 How much work does the CPU do for each type?

A register that is 32 bits wide can hold a 32-bit `int` in one go. Bigger types need extra work.

| Operation | 32-bit CPU | 64-bit CPU |
|---|---|---|
| Add two `int` (32-bit) | 1 instruction | 1 instruction |
| Add two `long long` (64-bit) | 2 steps (low half, then high half with carry) | 1 instruction |
| Hold a pointer | 1 register (32 bits) | 1 register (64 bits) |

Example with `unsigned long long`:

```
a = 4,000,000,000    b = 1,000,000,000

On a 32-bit CPU:
  The 64-bit sum (5,000,000,000) does not fit in a 32-bit register,
  so the compiler splits each number into two 32-bit halves and adds them in two steps.

On a 64-bit CPU:
  Both numbers and the result fit in one 64-bit register -> one add instruction.
```

Note: `float` and `double` are usually handled by separate floating-point registers in the CPU, so the 32-bit vs 64-bit width of normal registers matters mostly for integers and pointers.

### 8.5 Memory cost of bigger pointers

Because pointers are 8 bytes on 64-bit (instead of 4), data structures that store many pointers (linked lists, trees, graphs, hash maps) use more memory:

```cpp
struct Node {
    int   value;   // 4 bytes
    Node* next;    // 4 bytes on 32-bit, 8 bytes on 64-bit
};
```

```
32-bit: 4 + 4 = 8 bytes per node
64-bit: 4 + (4 padding) + 8 = 16 bytes per node
```

The padding is added so the 8-byte pointer sits at an address that is a multiple of 8. This is a good example of why **type size, alignment and architecture all work together.**

### 8.6 What the 4 GB limit means for types

A 32-bit process has at most about 4 GB of address space. So:

```cpp
int arr[1'000'000'000];   // 1 billion ints x 4 bytes = about 4 GB
```

This cannot work in a 32-bit process because the address space is too small, even before the OS reserves its share. Types and sizes decide how much memory you ask for, and the address width decides the maximum you can ever reach.

---

## 9. Overflow with data types

When a value is bigger than the type can hold, **overflow** happens.

### Unsigned overflow (wraps around, well defined)

```cpp
unsigned char x = 255;   // 11111111
x = x + 1;               // needs a 9th bit
cout << (int)x;          // 0   (wraps around: 100000000 keeps only the lowest 8 bits)
```

```
255 + 1 = 256 = 1 0000 0000  -> only the lowest 8 bits are kept -> 0000 0000 = 0
```

### Signed overflow (undefined behavior in C++)

```cpp
int a = 2147483647;      // INT_MAX = 01111111 11111111 11111111 11111111
a = a + 1;               // undefined behavior in C++
```

On many machines the bits wrap to `10000000 00000000 00000000 00000000`, which is -2,147,483,648, but the C++ standard does **not** promise this. Never rely on it.

### Choosing a bigger type

```cpp
long long total = 4'000'000'000LL + 1'000'000'000LL;   // 5,000,000,000 fits in 64 bits
```

Rule of thumb for backend code: if a number can pass about 2 billion (counts, IDs, timestamps in milliseconds, byte sizes), use `long long` or `int64_t`, not `int`.

---

## 10. Fixed-width types (safer for portable code)

Because `long` and `long double` change between systems, C++ gives fixed-size types in `<cstdint>`:

| Type | Size | Range |
|---|---|---|
| `int8_t` / `uint8_t` | 1 byte | -128 to 127 / 0 to 255 |
| `int16_t` / `uint16_t` | 2 bytes | -32,768 to 32,767 / 0 to 65,535 |
| `int32_t` / `uint32_t` | 4 bytes | about -2.1 x 10^9 to 2.1 x 10^9 / 0 to about 4.29 x 10^9 |
| `int64_t` / `uint64_t` | 8 bytes | about -9.2 x 10^18 to 9.2 x 10^18 / 0 to about 1.8 x 10^19 |

Use these when you need an exact size, such as in network protocols, file formats and databases.

---

## 11. Common mistakes to avoid

1. **"A `bool` uses 1 bit."** Wrong. It uses 1 full byte, because a byte is the smallest addressable unit.
2. **"`int` is always 4 bytes."** Mostly true on modern systems, but the standard does not guarantee it. Use `sizeof` or fixed-width types when it matters.
3. **"`long` is always 8 bytes on 64-bit."** Wrong. It is 4 bytes on 64-bit Windows and 8 bytes on 64-bit Linux/macOS.
4. **"64-bit makes every type bigger."** Wrong. Mainly pointers and `size_t` grow. `int`, `float` and `double` stay the same.
5. **"Unsigned can store bigger numbers."** Only in the positive direction. The total number of values is the same, just shifted.
6. **"`void` has a size you can use."** `void` is an incomplete type. You cannot make a `void` variable, only `void*` pointers and `void` return types.
7. **"Floats are exact."** Floats and doubles store approximations (limited precision), so avoid comparing them with `==` for money or exact values.

---

## 12. Quick revision table

| Type | Size | Bits | Range or note |
|---|---|---|---|
| `bool` | 1 byte | 8 | false / true |
| `char` | 1 byte | 8 | -128 to 127 (signed), 0 to 255 (unsigned) |
| `short` | 2 bytes | 16 | -32,768 to 32,767 |
| `unsigned short` | 2 bytes | 16 | 0 to 65,535 |
| `int` | 4 bytes | 32 | -2,147,483,648 to 2,147,483,647 |
| `unsigned int` | 4 bytes | 32 | 0 to 4,294,967,295 |
| `long` | 4 or 8 bytes | 32 or 64 | depends on OS and architecture |
| `long long` | 8 bytes | 64 | about -9 x 10^18 to 9 x 10^18 |
| `unsigned long long` | 8 bytes | 64 | 0 to about 1.8 x 10^19 |
| `float` | 4 bytes | 32 | +-1.2 x 10^-38 to +-3.4 x 10^38, 6 to 7 digits |
| `double` | 8 bytes | 64 | +-2.3 x 10^-308 to +-1.7 x 10^308, 15 to 17 digits |
| `long double` | 8, 12 or 16 bytes | 64 to 128 | extended precision |
| `void` | 0 (incomplete) | none | no value |
| pointer | 4 bytes (32-bit) / 8 bytes (64-bit) | 32 / 64 | an address |

**Formulas:**

```
Unsigned n-bit:  0 to 2^n - 1
Signed n-bit:    -2^(n-1) to 2^(n-1) - 1
```

**One-line summaries:**

- A data type tells the compiler the **size**, the **interpretation** and the **valid operations**.
- RAM only stores bits. The type gives the bits meaning.
- A 32-bit or 64-bit system mainly changes the **address width**, so pointers (and `size_t`) change size, while `int`, `float` and `double` usually do not.

---

## 13. Interview answers (ready to speak)

**Q: Why do we need data types?**

> RAM only stores raw bits and does not know what they mean. A data type tells the compiler how many bytes to reserve, how to interpret the bits (for example as a signed integer or an IEEE 754 float), and which operations are valid on them.

**Q: Why is `bool` 1 byte and not 1 bit?**

> The smallest addressable unit of memory is a byte, so even though a bool needs only one bit, it takes a full byte.

**Q: What changes between a 32-bit and a 64-bit system for data types?**

> The address width changes. Pointers and `size_t` are 4 bytes on 32-bit and 8 bytes on 64-bit. Types like `int`, `float` and `double` keep the same size, while `long` depends on the OS and architecture. A 64-bit CPU can also add 64-bit integers in one instruction, while a 32-bit CPU needs two steps.

**Q: How do you calculate the range of an integer type?**

> For n bits, unsigned goes from 0 to 2^n - 1, and signed (two's complement) goes from -2^(n-1) to 2^(n-1) - 1.

---

## 14. Self-check questions

1. Why can the bits `01000001` mean both 65 and 'A'?
2. What range does a signed 16-bit integer have, and how do you calculate it?
3. Why does a `bool` use a full byte?
4. Which types change size when you move from a 32-bit to a 64-bit system, and which do not?
5. Why is a `Node` with an `int` and a pointer bigger on a 64-bit system?
6. What happens when you add 1 to an `unsigned char` that holds 255?
7. Why is `long` called "architecture dependent"?
8. Why can you not create a variable of type `void`?
