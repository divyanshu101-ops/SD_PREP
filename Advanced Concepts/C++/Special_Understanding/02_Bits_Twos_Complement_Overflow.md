# Bits, Two's Complement, Integer Promotion, Shifts and Overflow in C++

*One connected flow. Each step uses the previous one. Most examples use `int8_t` (8 bits) because the bits are short enough to read. A real `int` works the same way with 32 bits.*

---

## 0. The full chain at a glance

```
bits in memory -> type decides meaning -> two's complement (signed int)
   -> arithmetic needs promotion -> converting back may wrap
   -> right shift on negatives -> unsigned wrap-around -> signed overflow (UB)
```

**Key facts these notes cover:**

- An `int` stores bits using **two's complement**. A `float` stores bits using the **IEEE 754** standard.
- Any integer type smaller than `int` (`char`, `unsigned char`, `short`, `int8_t`) is automatically **promoted to `int`** before any arithmetic or bitwise operation.
- Converting an out-of-range value to a signed type does not crash. In two's complement machines, `250` wraps to `-6`.
- `int8_t x = -5;` is stored in binary using two's complement.
- Right-shifting a negative number divides by 2 and rounds toward negative infinity (floor), so `-5 >> 1` gives `-3`.
- Unsigned overflow and underflow are completely well-defined by the C++ standard.
- `unsigned int` holding `0` minus `1` does not crash. It wraps to `4294967295`.

---

## 1. Bits are just bits: the type decides the meaning

RAM stores only 0s and 1s. The same pattern can mean different things:

```
Bits: 11111011

as unsigned char -> 251
as int8_t        -> -5
as part of float -> no meaning alone, a float needs 32 bits
```

So "what is stored" and "what it means" are two separate things. Everything below follows from this.

**Useful sizes and ranges (n bits):**

```
Unsigned:  0 to 2^n - 1
Signed:    -2^(n-1) to 2^(n-1) - 1

8 bits : unsigned 0 to 255,        signed -128 to 127
16 bits: unsigned 0 to 65,535,     signed -32,768 to 32,767
32 bits: unsigned 0 to 4,294,967,295, signed -2,147,483,648 to 2,147,483,647
```

---

## 2. How two's complement works

**Idea:** in an n-bit signed number, the top bit has a **negative weight**.

```
8-bit weights:   -128   64   32   16    8    4    2    1
```

Positive numbers have the top bit 0, so they read normally. If the top bit is 1, you start from -128 and add the rest.

### 2.1 Making -5 from 5 (the 3-step recipe)

```
Step A: write +5            00000101
Step B: invert all bits     11111010
Step C: add 1               11111011   <- this is -5
```

### 2.2 Verify by reading the weights

```
11111011
-128 + 64 + 32 + 16 + 8 + 0 + 2 + 1
= -128 + 123
= -5   OK
```

### 2.3 The same for a 32-bit `int`

```
+5 : 00000000 00000000 00000000 00000101
-5 : 11111111 11111111 11111111 11111011     (hex 0xFFFFFFFB)
```

`int8_t x = -5;` is stored as `11111011` (hex `0xFB`).

### 2.4 All 8-bit patterns around zero

| Bits | Value |
|---|---|
| 01111111 | 127 (biggest) |
| 00000001 | 1 |
| 00000000 | 0 |
| 11111111 | -1 |
| 11111011 | -5 |
| 10000000 | -128 (smallest) |

### 2.5 Why this design? One adder does everything

**Addition:** 5 + (-5)

```
  00000101    (+5)
+ 11111011    (-5)
----------
1 00000000    -> the 9th bit (carry) is thrown away -> 00000000 = 0   OK
```

**Subtraction is just addition:** 7 - 5 = 7 + (-5)

```
  00000111    (+7)
+ 11111011    (-5)
----------
1 00000010    -> drop the carry -> 00000010 = 2   OK
```

No separate subtraction circuit is needed. That is the reason `int` uses two's complement.

---

## 3. `int` (two's complement) vs `float` (IEEE 754)

Same value, completely different bits.

**-5 as `int`:**

```
11111111 11111111 11111111 11111011     (0xFFFFFFFB)
```

**-5.0 as `float`:**

```
5.0 = 101.0 in binary = 1.01 x 2^2

sign     = 1                      (negative)
exponent = 2 + 127 = 129 = 10000001
mantissa = 01 then zeros = 01000000000000000000000

1 | 10000001 | 01000000000000000000000      (hex 0xC0A00000)
```

**Difference in negation:**

- `int`: invert all bits and add 1.
- `float`: just flip the sign bit. `+5.0f` is `0x40A00000`, `-5.0f` is `0xC0A00000`.

So the CPU needs different instructions for the two, and the compiler picks them based on the type.

---

## 4. Integer promotion

Types smaller than `int` (`char`, `unsigned char`, `short`, `int8_t`) are **converted to `int` first** before any arithmetic or bitwise operation.

### 4.1 How the widening happens

```
int8_t        -5  = 11111011
promoted to int   = 11111111 11111111 11111111 11111011   (sign extension: copy the top bit)
                    value still -5   OK

unsigned char 251 = 11111011   (same bits!)
promoted to int   = 00000000 00000000 00000000 11111011   (zero extension: fill with 0)
                    value 251   OK
```

Same 8 bits, different results, because the type decides how to fill.

### 4.2 Example A: the addition happens in `int`

```cpp
unsigned char a = 200, b = 100;
auto r = a + b;          // r is int: 300, not 44
unsigned char c = a + b; // 300 does not fit in 8 bits -> goes to section 5
```

### 4.3 Example B: bitwise NOT

```cpp
unsigned char u = 5;     // 00000101
auto n = ~u;             // promoted first: ~(00000000 00000000 00000000 00000101)
                         //               = 11111111 11111111 11111111 11111010
                         // n is int with value -6
unsigned char back = ~u; // keeps the low 8 bits = 11111010 = 250
```

Many people expect `~u` to be 250. It is -6 until you store it back into an 8-bit type.

---

## 5. Converting back to a small type (out of range)

After promotion, the result is an `int`. Storing it into a smaller type keeps only the **lowest n bits**. In other words it wraps modulo 2^n.

### 5.1 Unsigned target: 300 into `unsigned char`

```
300 = 1 0010 1100
keep low 8 bits  ->  0010 1100 = 44
(300 mod 256 = 44)
```

### 5.2 Signed target: 250 into `int8_t`

```
250 = 11111010   (fits in 8 bits as a pattern)
read as signed: -128 + 64 + 32 + 16 + 8 + 2 = -6
(250 - 256 = -6)
```

### 5.3 Combined example: promotion then conversion

```cpp
int8_t a = 100, b = 100;
int8_t c = a + b;
```

```
a + b is done in int: 200
200 = 11001000
stored in int8_t: -128 + 64 + 8 = -56     (200 - 256 = -56)
```

### 5.4 What does the standard say?

- Before C++20, converting an out-of-range value to a signed type was **implementation-defined**. Every mainstream compiler wrapped it as shown above.
- Since C++20, the wrap-around is **fully defined** (modulo 2^n).
- Either way, it does not crash. Do not confuse this with signed arithmetic overflow (section 8), which is different.
- `int8_t x = 250;` usually gives a warning. `int8_t x{250};` is a compile error (narrowing).

---

## 6. Right shift on negative numbers

On signed types, `>>` is an **arithmetic shift**: it copies the sign bit into the new empty position. (The standard made this fully defined in C++20. Before that it was implementation-defined, but all mainstream compilers did it this way.)

```
-5 >> 1:

  11111011      (-5)
   shift right by 1, copy the sign bit (1) on the left
  11111101      = -128 + 64 + 32 + 16 + 8 + 4 + 1 = -3
```

Shifting right by 1 means `floor(x / 2)`:

```
-5 / 2 = -2.5  ->  floor = -3   OK  (matches the shift)
```

### 6.1 Important: `>> 1` and `/ 2` are not the same for negatives

```cpp
int x = -5;
cout << (x >> 1) << endl;  // -3   (floor, toward negative infinity)
cout << (x / 2)  << endl;  // -2   (division truncates toward zero since C++11)
```

| Expression | Rounds | Result for -5 |
|---|---|---|
| `x >> 1` | toward negative infinity | -3 |
| `x / 2` | toward zero | -2 |

For positive numbers both give the same answer. For negative odd numbers they differ.

### 6.2 Unsigned shift is a logical shift (fills with 0)

```
unsigned char 251 = 11111011
251 >> 1 = 01111101 = 125
```

The same bits `11111011` gave -3 as signed (after the shift) and 125 as unsigned. The type decides again.

---

## 7. Unsigned wrap-around (well-defined)

Unsigned arithmetic is always **modulo 2^n**, and the C++ standard guarantees it. No error, no crash.

### 7.1 0 - 1 for `unsigned int`

```cpp
unsigned int z = 0;
z = z - 1;     // 4294967295
```

Subtraction is addition of the two's complement of 1. The two's complement of 1 is all ones:

```
  00000000 00000000 00000000 00000000    (0)
+ 11111111 11111111 11111111 11111111    (-1 pattern, two's complement of 1)
------------------------------------
  11111111 11111111 11111111 11111111    = 4294967295 = 2^32 - 1
```

The adder just produced `0xFFFFFFFF`. As `int` this pattern means -1. As `unsigned int` it means 4294967295. Same bits, different reading.

### 7.2 Same idea on 8 bits

```cpp
unsigned char c = 255;
c = c + 1;      // 255 + 1 = 256 -> keep low 8 bits -> 0
```

### 7.3 Classic bugs from this

```cpp
// Bug 1: infinite loop
for (unsigned int i = 5; i >= 0; i--) { }   // i >= 0 is always true; at 0, i-- becomes 4294967295

// Bug 2: empty vector
std::vector<int> v;
size_t last = v.size() - 1;   // 0 - 1 -> huge number (2^64 - 1 on 64-bit), not -1

// Bug 3: mixed comparison
int a = -1; unsigned int b = 1;
if (a < b) { }     // false! a is converted to unsigned: 4294967295 < 1 is false
```

**Link to 32-bit vs 64-bit:** `size_t` is 32 bits on a 32-bit system and 64 bits on a 64-bit system, so the "huge number" in Bug 2 differs (4294967295 vs 18446744073709551615).

---

## 8. Signed overflow (undefined behavior)

This is different from section 5 (conversion) and section 7 (unsigned).

```cpp
int a = 2147483647;   // INT_MAX = 01111111 11111111 11111111 11111111
a = a + 1;            // undefined behavior
```

The hardware would produce `10000000 ...` = -2147483648, but the C++ standard does **not** promise it. The compiler may assume it never happens and optimize in surprising ways.

| Case | Defined? |
|---|---|
| Unsigned arithmetic overflow (`0u - 1`) | Yes, wraps modulo 2^n |
| Out-of-range value converted to signed type (`int8_t x = 250`) | Implementation-defined before C++20, defined (wraps) since C++20 |
| Signed arithmetic overflow (`INT_MAX + 1`) | **Undefined behavior** |

---

## 9. One full trace using everything

```cpp
unsigned char u = 250;        // (1)
int8_t s = (int8_t)u;         // (2)
int t = s >> 1;               // (3)
int r = s + 10;               // (4)
unsigned int z = 0;
z = z - 1;                    // (5)
```

```
(1) u = 250 = 11111010

(2) conversion to signed: same 8 bits, read with a negative top weight
    s = 11111010 = -6

(3) s is promoted to int (sign extension):
    11111111 11111111 11111111 11111010   (-6)
    >> 1 (arithmetic, copy sign bit):
    11111111 11111111 11111111 11111101   = -3     (floor(-6/2) = -3)

(4) s promoted to int = -6
    -6 + 10 = 4
      11111111 11111111 11111111 11111010
    + 00000000 00000000 00000000 00001010
    = 1 00000000 00000000 00000000 00000100  -> drop carry -> 4   OK

(5) z = 0xFFFFFFFF = 4294967295
```

---

## 10. Final flow in one view

```
1. Memory holds only bits.
2. Type decides the reading:
      signed int   -> two's complement (top bit has negative weight)
      float/double -> IEEE 754 (sign | exponent | mantissa)
      unsigned     -> plain binary
3. Two's complement negation = invert bits + 1,
   so subtraction is just addition on the same adder.
4. Small types (char, short, int8_t) are promoted to int before math or bit ops:
      signed -> sign-extended, unsigned -> zero-extended.
5. Storing a big int back into a small type keeps only the low n bits (wraps).
6. Right shift:
      signed   -> arithmetic (copies sign bit) -> floor division by 2^k
      unsigned -> logical (fills 0)
      note: x >> 1 floors, x / 2 truncates toward zero.
7. Unsigned overflow/underflow wraps (well-defined): 0u - 1 = 4294967295.
8. Signed arithmetic overflow is undefined behavior: INT_MAX + 1.
```

---

## 11. Common mistakes

1. **"`-5 / 2` is -3."** Wrong. Integer division truncates toward zero, so `-5 / 2` is `-2`. Only `-5 >> 1` gives `-3`.
2. **"`~u` on an `unsigned char` is 250."** It is `-6` (an `int`) until you store it back into an 8-bit type.
3. **"`a + b` of two `unsigned char` is an `unsigned char`."** It is an `int`, because of promotion.
4. **"All overflow is the same."** No. Unsigned wraps (defined), conversion to signed wraps (defined in C++20), signed arithmetic overflow is undefined behavior.
5. **"Unsigned is safer."** Not always. `0u - 1` silently becomes a huge number, which causes infinite loops and wrong comparisons.
6. **"Same bits mean the same value."** No. The type decides: `11111011` is 251, -5, or part of a float depending on the type.

---

## 12. Quick revision table

| Concept | Rule | Example |
|---|---|---|
| Two's complement negate | invert bits, add 1 | 5 = 00000101 -> -5 = 11111011 |
| Signed range | -2^(n-1) to 2^(n-1) - 1 | 8-bit: -128 to 127 |
| Unsigned range | 0 to 2^n - 1 | 8-bit: 0 to 255 |
| Float negate | flip sign bit | 0x40A00000 -> 0xC0A00000 |
| Promotion | small types become `int` | `a + b` of two `unsigned char` is `int` |
| Sign extension | copy top bit (signed) | 11111011 -> 11111111 ... 11111011 |
| Zero extension | fill with 0 (unsigned) | 11111011 -> 00000000 ... 11111011 |
| Narrowing conversion | keep low n bits | 300 -> 44 (unsigned char), 250 -> -6 (int8_t) |
| Signed `>>` | arithmetic, floor | -5 >> 1 = -3 |
| Unsigned `>>` | logical, fill 0 | 251 >> 1 = 125 |
| Integer `/` | truncate toward zero | -5 / 2 = -2 |
| Unsigned underflow | wraps, defined | 0u - 1 = 4294967295 |
| Signed overflow | undefined behavior | INT_MAX + 1 |

---

## 13. Self-check questions

1. Write -5 in 8-bit two's complement using the invert-and-add-1 recipe. Verify it with the bit weights.
2. Why does the CPU not need a separate subtraction circuit?
3. What are the bits of `-5.0f`, and how does negating a `float` differ from negating an `int`?
4. What is the type of `a + b` when both are `unsigned char`? Why?
5. What is the difference between sign extension and zero extension? Give the example of `11111011`.
6. What does `int8_t c = 100 + 100;` store, and why?
7. Why is `-5 >> 1` equal to `-3` but `-5 / 2` equal to `-2`?
8. What does `unsigned int z = 0; z--;` give, and is it a crash, an error, or defined behavior?
9. Why is `for (unsigned i = 5; i >= 0; i--)` a bug?
10. Which of these is undefined behavior: `0u - 1`, `int8_t x = 250`, `INT_MAX + 1`?
