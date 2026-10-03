# 32-bit vs 64-bit: Simple Notes

*Computer architecture basics for backend and system design. Read once fully, then use the revision table at the end.*

## 1. The one-line idea

**N-bit means the CPU's registers are N bits wide. The wider the register, the bigger the address it can hold, and the more memory the CPU can point to.**

- 32-bit CPU: registers are 32 bits wide.
- 64-bit CPU: registers are 64 bits wide.

## 2. What is a register?

A **register** is a tiny, very fast storage box inside the CPU. The CPU does its work on registers, not directly on RAM.

- RAM is big but slow compared to the CPU.
- Registers are very few in number but extremely fast.
- The CPU copies data from RAM into registers, works on it, and writes the result back.

Think of RAM as a big warehouse and registers as the table in front of you. You bring items from the warehouse to the table, work on them, and send them back.

## 3. What does "32-bit wide" mean?

A bit is one position that can be 0 or 1. A 32-bit register has 32 such positions.

```
32-bit register = 32 cells, each cell holds 0 or 1

00000000 00000000 00000000 00000000
|-8 bits-| |-8 bits-| |-8 bits-| |-8 bits-|
          = 4 bytes = 32 bits
```

A 64-bit register is the same idea, but with 64 positions (8 bytes).

**How many different values can a register hold?** Each position has 2 choices (0 or 1), so:

| Register | Positions | Different values | Biggest unsigned value |
|---|---|---|---|
| 32-bit | 32 | 2^32 = 4,294,967,296 | 4,294,967,295 |
| 64-bit | 64 | 2^64 = 18,446,744,073,709,551,616 | 18,446,744,073,709,551,615 |

## 4. A register holds two kinds of things

This is the most important point. A register does not only store addresses.

1. **Data (a normal number):** for example a = 5.
2. **An address (a location in RAM):** for example "the data lives at address 4096".

The bits look the same. What changes is how the CPU uses them.

**Example with a small 8-bit register (easy to read):**

```
R1 holds DATA:     00000101  = 5
R2 holds ADDRESS:  00001010  = 10   (meaning: go to RAM location 10)
```

Same kind of box, different meaning.

## 5. Flow: how data comes from RAM into a register

Each register is 32 bits wide in a 32-bit CPU and 64 bits wide in a 64-bit CPU.

```
            CPU                                        RAM
  +--------------------------+              +---------------------------+
  | R2 (address register)    |  1. send     | Address 4095: other data  |
  |   holds 4096             |-- address -->|                           |
  |                          |   4096       | Address 4096: 25   <------+-- the target cell
  | R1 (data register)       |<-- 2. data --|                           |
  |   holds 25               |   returns    | Address 4097: other data  |
  |                          |              |                           |
  | R3 (data register)       |              +---------------------------+
  |   holds 10               |
  |                          |   3. The CPU adds R1 + R3 = 35
  +--------------------------+   4. Result is saved back, so address 4096 now holds 35
```

R2 holds the address. The CPU sends it to RAM (step 1), RAM sends back the value stored there (step 2), and the CPU can now work on it in a register.

## 6. Step-by-step example of the flow

Program: *"Add 10 to the number stored at RAM address 4096, then save it back."* Assume RAM address 4096 currently holds 25.

```
RAM before:  Address 4096 -> 25

Step 1: Put the address in a register
        R2 = 4096            (R2 holds an ADDRESS)

Step 2: Load data from that address into another register
        R1 = RAM[R2]         (CPU sends 4096 to RAM, RAM sends back 25)
        R1 = 25              (R1 holds DATA)

Step 3: Put the second number in a register
        R3 = 10              (R3 holds DATA)

Step 4: Do the calculation
        R1 = R1 + R3         (25 + 10 = 35)

Step 5: Store the result back to RAM
        RAM[R2] = R1         (address 4096 now holds 35)

RAM after:   Address 4096 -> 35
```

R2 is used as an address. R1 and R3 are used as data. All three are normal registers.

**Binary view of the same example (32-bit registers):**

```
R2 (address 4096):  00000000 00000000 00010000 00000000
R1 (data 25):       00000000 00000000 00000000 00011001
R3 (data 10):       00000000 00000000 00000000 00001010
R1 after add (35):  00000000 00000000 00000000 00100011
```

In a 64-bit CPU each register has 32 more zeros on the left, but the idea is exactly the same.

## 7. Why the width limits memory: the street analogy

Think of RAM as a long street. Every house holds **1 byte** and has a unique house number (the **address**).

```
Address:   0      1      2      3      4     ...
Data:    [0x41] [0x00] [0xFF] [0x10] [0x7A]  ...
```

To reach a house, the CPU must put its house number in a register. So:

**The biggest house number = the biggest value the register can hold.**

**Small example: a 4-bit CPU (a toy, just to see the pattern)**

```
4 positions -> 2 x 2 x 2 x 2 = 16 addresses

0000 = address 0
0001 = address 1
0010 = address 2
 ...
1111 = address 15   <- the last one. Address 16 would need a 5th bit.
```

So a 4-bit CPU can reach only 16 bytes of memory, even if you plug in more RAM.

**Now scale it up:**

| CPU | Address bits | Unique addresses | Max memory (1 address = 1 byte) |
|---|---|---|---|
| 4-bit (toy) | 4 | 2^4 = 16 | 16 bytes |
| 32-bit | 32 | 2^32 = 4,294,967,296 | 4 GB |
| 64-bit | 64 | 2^64 | 16 Exabytes (in theory) |

**Real-life version:** if house numbers can have only 3 digits, you can have at most 1000 houses (000 to 999). Adding more land does not help, because you cannot write a number for the extra houses.

So a 32-bit CPU with 8 GB of RAM can still use only about 4 GB, because it cannot write an address bigger than 32 bits.

## 8. First and last address in 32-bit

```
First address:  00000000 00000000 00000000 00000000  = 0
Last address:   11111111 11111111 11111111 11111111  = 4,294,967,295
```

In hex (a shorter way to write binary):

```
32-bit last address: 0xFFFFFFFF            (8 hex digits)
64-bit last address: 0xFFFFFFFFFFFFFFFF    (16 hex digits)
```

There is no "next" address after all 1s. Adding 1 would need a 33rd bit, which does not exist. This is called **overflow**.

## 9. Overflow in a calculation

Registers also do math, so the width limits the size of numbers too.

```
32-bit max value = 4,294,967,295

Try: 4,000,000,000 + 1,000,000,000 = 5,000,000,000

5,000,000,000 is bigger than 4,294,967,295, so it does not fit in 32 bits.
The result wraps around:
5,000,000,000 - 4,294,967,296 = 705,032,704   (wrong answer in a 32-bit unsigned register)
```

A 32-bit CPU can still work with bigger numbers, but it must split them into two parts and use more instructions. A 64-bit CPU fits the number in one register, so it is faster for big values.

## 10. Code example (C++): pointer size

A pointer stores an address, so its size equals the address width.

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 10;
    int* p = &x;   // p stores the address of x

    cout << "Pointer size: " << sizeof(p) << " bytes" << endl;
    cout << "Address: " << p << endl;
}
```

- On a **32-bit** build: pointer size is **4 bytes**. Example address: `0xffb2a1c4` (8 hex digits).
- On a **64-bit** build: pointer size is **8 bytes**. Example address: `0x7ffd5e3a1b2c` (longer).

The exact address changes on every run. Only the size matters here.

## 11. Operating system view

The OS gives every process its own **virtual address space**, so each program feels like it owns all the memory.

**32-bit OS**

- Each process can use at most about 4 GB of virtual addresses.
- Part of that is reserved for the kernel, so a program usually gets about 2 to 3 GB for its own code, heap and stack.
- This is true even if the machine has 16 GB of RAM.

**64-bit OS**

- The address space is huge. Today's CPUs actually use 48 of the 64 bits, which is about 256 TB per process.
- Programs can load very large data (big caches, databases, AI models).

**Page tables:** the OS uses page tables to translate a virtual address into a real RAM address. Bigger addresses need bigger, multi-level tables (commonly 4 levels on 64-bit, versus 2 levels on 32-bit x86).

## 12. Why is it called computer architecture?

Because it is a **hardware design choice**. The design is called the **ISA (Instruction Set Architecture)**. It defines the register sizes and the instructions the CPU understands.

- 32-bit examples: x86 (older Intel), ARMv7.
- 64-bit examples: x86-64 / AMD64, ARM64 (Apple M-series, modern phones).

A 64-bit OS cannot run on a 32-bit CPU, because the hardware does not understand 64-bit instructions. The other way works: a 64-bit CPU can usually run 32-bit programs.

## 13. Why this matters for a backend engineer

- **Redis and in-memory databases:** a 32-bit build is limited to about 4 GB of data. Production uses 64-bit.
- **Memory-heavy servers:** caches, big queues and loading AI models need a 64-bit system.
- **Trade-off:** pointers are 2x bigger in 64-bit (8 bytes vs 4), so programs use a little more memory.

## 14. Common mistakes to avoid

1. **"A register only stores addresses."** Wrong. It stores both data and addresses.
2. **"More RAM always means more usable memory."** Wrong for 32-bit. The address width is the limit.
3. **"32-bit means 32 bytes."** Wrong. It means 32 bits (4 bytes) per register.
4. **"A pattern like 0101 repeated is the limit."** Wrong. Each bit pattern is just one address. The limit is how many different patterns exist: 2^32 or 2^64.

## 15. Interview answer (ready to speak)

> A CPU executes instructions one by one, using registers to hold data and memory addresses. In a 32-bit CPU the registers are 32 bits wide, so it can form 2^32 unique addresses, which is 4 GB of memory. In a 64-bit CPU the registers are 64 bits wide, so it can form 2^64 addresses, a far larger range. The CPU puts an address in a register, fetches the data from RAM at that address, and then operates on it. This width is part of the CPU architecture (ISA), and it affects the OS through virtual address space size and page table design.

## 16. Quick revision table

| Point | 32-bit | 64-bit |
|---|---|---|
| Register width | 32 bits (4 bytes) | 64 bits (8 bytes) |
| Unique addresses | 2^32 | 2^64 |
| Max addressable memory | 4 GB | 16 EB (in theory) |
| Pointer size | 4 bytes | 8 bytes |
| Last address (hex) | 0xFFFFFFFF | 0xFFFFFFFFFFFFFFFF |
| Biggest unsigned number | 4,294,967,295 | about 1.8 x 10^19 |
| Process address space | about 4 GB | about 256 TB in practice |
| Typical use today | old or tiny devices | servers, laptops, phones |

## 17. Self-check questions

1. What is the last address a 32-bit register can hold, and why is there no next one?
2. If a machine has 8 GB RAM and a 32-bit CPU, how much can one process address?
3. Why is a pointer 8 bytes on 64-bit?
4. Name two things a register can store.
