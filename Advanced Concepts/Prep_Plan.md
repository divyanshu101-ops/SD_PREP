# Preparation: Complete Topic List and 3-Day / 4-Night Plan

Scope: C++, OOPs, Data Structures and Algorithms, Operating Systems, Computer Networks, Linux, DBMS, Project and HR.
Goal: go from "surface knowledge" to "internal understanding" in 4 nights and 3 days.

---

## 1. What Nokia Usually Asks (Pattern From Past Candidate Reports)

- Online assessment: MCQs plus coding questions (technical MCQs on OS, C/C++, data structures, networking; sometimes quants, verbal, logical).
- Technical rounds (1 to 3 rounds, about 60 to 90 minutes each): C++/OOPs, DSA (easy to medium), OS, Computer Networks, Linux, DBMS, and a deep dive into resume projects.
- Role-dependent depth: embedded / R&D / systems roles go deeper into OS, C++ internals, networking, Linux and RTOS.
- Reported question areas: semaphores and mutex, concurrency problems, zombie process, process communication, interrupts, Linux commands, OSI layers, IP addressing, operator overloading, constructors and destructors, linked list cycle detection and removal, Dijkstra variations, palindrome and subsets problems, array and string problems, project discussion.
- HR round: why Nokia, strengths and weaknesses, teamwork, family background, behavioural questions.
- Nokia tends to reward depth and clear fundamentals over broad but shallow coverage.

---

## 2. Analysis of Your Assessment Questions (Mapped to Topics)

### 2.1 Operating Systems
- Process synchronization at hardware and software levels (TSL, CAS, Peterson, semaphores, mutex, monitors).
- File system allocation: contiguous, linked, indexed (single-level index per file), multilevel index, inode.
- Deadlock: resource-count numericals (formula: `p * (need - 1) + 1 <= resources` for deadlock-free), prevention, avoidance (Banker's), detection, recovery.
- Shared libraries in an RTOS: effect on memory, disk space, maintenance; static vs dynamic linking.
- IPC and message passing: minimum primitives are send and receive.
- CPU scheduling: Round Robin and the time quantum choice versus burst times; turnaround, waiting and response time.
- Concurrency: race condition (output depends on execution order).
- Windows-specific trivia (UWP media controls such as IsChannelDownEnabled, kernel debugging flags, tools.ini): very low priority; only skim if time remains.

### 2.2 Data Structures and Algorithms
- Range sum queries with point updates: Fenwick Tree (BIT), also Segment Tree.
- Hash table for storing and searching anagrams.
- Merge Sort complexity: O(n log n) time, O(n) space.
- Rabin-Karp: rolling hash, O(n + m) expected time.
- Horner's method: polynomial of degree n needs n multiplications.
- Kruskal MST with DSU: O(E log V).
- KMP prefix (LPS) array: O(m) to build, O(n + m) total.
- Dijkstra with min-heap: O((V + E) log V).
- Sorting benchmark: Bubble Sort to Quick Sort gives the biggest asymptotic gain on random data.
- C++ sort optimisation: cache derived keys (Schwartzian transform / decorate-sort-undecorate).

### 2.3 Coding Problems Seen
- Perfect-square product pairs (i, j) in 1..N: number theory, square-free part, sieve, grouping by square-free kernel.
- Circular doubly linked list with XOR of adjacent nodes, parity check and deletions: pointer manipulation, careful simulation, ignore distractor sub-statements.

---

## 3. Complete Topic Checklist

### 3.1 C++ and OOPs

#### Language basics
- Data types, type modifiers, size and range
- Type casting: static_cast, dynamic_cast, const_cast, reinterpret_cast, C-style cast
- References vs pointers; pointer to pointer; function pointers; pointer arithmetic; array decay
- const correctness (const pointer, pointer to const, const member function)
- auto, decltype, constexpr, consteval, inline, static, extern, volatile, mutable, register
- Namespaces, scope resolution, using declarations
- Preprocessor, macros vs inline functions, header guards, #pragma once
- Compilation stages: preprocessing, compilation, assembly, linking; static vs dynamic linking
- Name mangling, extern "C"
- Undefined behaviour, unspecified and implementation-defined behaviour
- Operator precedence and associativity, sequence points, evaluation order
- Bit fields, unions, enums (enum class), structs vs classes, typedef vs using

#### Memory management
- Program memory layout: text, data, BSS, heap, stack
- Stack vs heap; new/delete vs malloc/free; new[] vs delete[]; placement new
- Memory leak, dangling pointer, wild pointer, double free, use-after-free, buffer overflow
- sizeof for structs and classes, padding, alignment, empty class size, class with virtual functions size
- Endianness
- Custom allocators (concept)

#### Classes and objects
- Class, object, access specifiers
- Constructors: default, parameterised, copy, move, delegating, explicit
- Destructors and order of construction/destruction
- Member initialiser list (and why it is needed)
- this pointer
- Static members and static member functions
- friend functions and friend classes
- Shallow copy vs deep copy
- Copy assignment vs copy constructor
- Rule of 0, 3 and 5
- Copy elision, RVO, NRVO
- Object slicing

#### Four pillars
- Encapsulation, abstraction, inheritance, polymorphism
- Inheritance types: single, multilevel, multiple, hierarchical, hybrid
- Public, protected, private inheritance
- Diamond problem and virtual inheritance
- Name hiding and function hiding vs overriding
- Compile-time polymorphism: function overloading, operator overloading (all overloadable operators, restrictions, member vs non-member, ++/-- prefix vs postfix, <<, >>, [], (), ->, =)
- Runtime polymorphism: virtual functions, vtable and vptr, pure virtual functions, abstract classes, interfaces
- Virtual destructor and why it matters
- override and final keywords
- Constructors and virtual calls inside constructors
- Covariant return types
- RTTI, typeid, dynamic_cast

#### Modern C++ (C++11 to C++20)
- lvalue, rvalue, xvalue, prvalue
- Move semantics, std::move, std::forward, perfect forwarding, universal references
- Smart pointers: unique_ptr, shared_ptr, weak_ptr, circular reference problem, make_unique, make_shared
- RAII
- Lambdas: captures, mutable, generic lambdas
- nullptr, range-based for, structured bindings, initializer lists, uniform initialisation
- std::optional, std::variant, std::any, std::string_view, std::span
- constexpr functions, if constexpr
- Concepts and ranges (basic awareness)
- noexcept, deleted and defaulted functions

#### Templates
- Function templates, class templates
- Template specialisation (full and partial)
- Variadic templates, fold expressions
- Template metaprogramming basics, SFINAE
- Templates vs macros; template instantiation and code bloat

#### STL
- Containers: vector, array, deque, list, forward_list, set, multiset, map, multimap, unordered_set, unordered_map, stack, queue, priority_queue
- Internals: vector growth and capacity, iterator invalidation rules, map as red-black tree, unordered_map hashing and collisions, deque layout
- Iterators and their categories
- Algorithms: sort, stable_sort, partial_sort, nth_element, binary_search, lower_bound, upper_bound, unique, reverse, rotate, next_permutation, accumulate, find, count, transform, for_each, min/max_element
- Comparators, functors, custom hash
- Complexity of every container operation
- std::string internals, small string optimisation

#### Exception handling
- try, catch, throw, rethrow
- Stack unwinding
- Exceptions in constructors and destructors
- noexcept, standard exception hierarchy
- Exception safety guarantees

#### Concurrency in C++
- std::thread, join vs detach
- mutex, lock_guard, unique_lock, scoped_lock, recursive_mutex
- condition_variable
- atomic, memory ordering (basic)
- future, promise, async, packaged_task
- Race condition, deadlock, livelock, thread pool
- Thread-safe singleton

#### Design and misc
- SOLID principles
- Design patterns: Singleton, Factory, Abstract Factory, Observer, Strategy, Builder, Adapter, Decorator
- File I/O, streams, stringstream
- Common output-prediction traps (pointers, sizeof, static, virtual, inheritance, overloading resolution)

---

### 3.2 Data Structures and Algorithms

#### Complexity analysis
- Big-O, Omega, Theta
- Best, average, worst case
- Amortised analysis
- Space complexity, recursion stack space
- Recurrence relations, Master theorem
- Complexity table of all common algorithms (keep one page)

#### Arrays and strings
- Two pointers, sliding window (fixed and variable)
- Prefix sum, difference array
- Kadane's algorithm, Dutch national flag, Boyer-Moore voting
- Matrix traversal, rotation, spiral
- Palindrome, subsequence, subset, anagram, permutation problems
- String algorithms: KMP, Z-algorithm, Rabin-Karp, Manacher, Trie, suffix array (basic), rolling hash

#### Hashing
- Hash table, hash functions
- Collision handling: chaining, open addressing (linear, quadratic, double hashing)
- Load factor, rehashing
- Frequency maps, grouping anagrams

#### Linked lists
- Singly, doubly, circular, circular doubly
- Reverse (iterative, recursive, in groups of k)
- Cycle detection and removal (Floyd), cycle start
- Middle node, nth from end, merge two lists, intersection
- Palindrome list, flattening, copy list with random pointer
- LRU cache design
- XOR linked list concept

#### Stacks and queues
- Monotonic stack and queue, next greater/smaller element
- Infix, prefix, postfix conversion and evaluation
- Min stack, queue using stacks, stack using queues
- Circular queue, deque, priority queue

#### Recursion and backtracking
- Subsets, permutations, combinations, combination sum
- N-Queens, Sudoku, rat in a maze
- Palindrome partitioning, word break

#### Searching and sorting
- Binary search, binary search on answer, rotated array search
- Bubble, selection, insertion, merge, quick, heap, counting, radix, bucket sort
- Stability, in-place, adaptive properties, best/worst cases
- Quickselect, k-th largest
- External sorting (concept)

#### Trees
- Binary tree traversals (recursive, iterative, Morris, level order)
- BST operations, validation, kth smallest, LCA
- AVL tree and rotations, red-black tree (concept), B and B+ trees (concept)
- Diameter, height, balanced check, views, zigzag, boundary traversal
- Serialise and deserialise, construct from traversals
- Heap: build-heap, heapify, heap sort, top-K, median of a stream, merge K sorted lists
- Trie, Segment Tree (with lazy propagation), Fenwick Tree (BIT), Sparse Table
- Range sum with point update, range min/max queries

#### Graphs
- Representations: adjacency list, matrix
- BFS, DFS, connected components
- Cycle detection (directed and undirected)
- Topological sort (DFS and Kahn)
- Shortest paths: Dijkstra, Bellman-Ford, Floyd-Warshall, 0-1 BFS, Dijkstra variations (path count, constrained paths)
- MST: Prim, Kruskal
- Disjoint Set Union with rank and path compression
- SCC (Kosaraju, Tarjan), bridges, articulation points
- Bipartite check, flood fill, grid graphs, word ladder

#### Dynamic programming
- 1D and 2D DP, memoisation vs tabulation
- Knapsack variants (0/1, unbounded, subset sum, partition)
- LIS, LCS, edit distance, longest palindromic subsequence
- Matrix chain multiplication, MCM-style DP
- Coin change, rod cutting, house robber
- DP on grids, strings, trees, bitmask
- Digit DP (basic awareness)

#### Greedy
- Activity selection, interval scheduling and merging
- Huffman coding, fractional knapsack
- Job sequencing, minimum platforms, gas station

#### Bit manipulation
- AND, OR, XOR, shifts, two's complement
- Set, clear, toggle, check bit
- Count set bits, power of two, single number problems
- Subset generation via bitmask

#### Math and number theory
- Sieve of Eratosthenes, segmented sieve, smallest prime factor sieve
- GCD, LCM, extended Euclid
- Modular arithmetic, fast exponentiation, modular inverse
- Prime factorisation, square-free numbers, perfect squares, divisor counting
- nCr mod p, Catalan numbers, inclusion-exclusion
- Horner's method, polynomial evaluation
- Basic probability and combinatorics

#### Design problems
- LRU and LFU cache, min stack, rate limiter, iterator design, hash map from scratch

---

### 3.3 Operating Systems

#### Introduction
- OS goals and functions
- Types: batch, multiprogramming, time-sharing, real-time (hard/soft), distributed, embedded
- Kernel mode vs user mode, system calls, interrupts and traps
- Monolithic vs microkernel vs hybrid
- Boot process (BIOS/UEFI, bootloader, kernel init)

#### Processes and threads
- Process concept, PCB, process states and transitions
- Context switch and its cost
- fork, exec, wait, exit; fork-related output questions
- Zombie, orphan and daemon processes
- Process vs thread; user-level vs kernel-level threads; multithreading models (many-to-one, one-to-one, many-to-many)
- Thread safety, thread-local storage

#### CPU scheduling
- FCFS, SJF, SRTF, Priority (preemptive and non-preemptive), Round Robin, multilevel queue, multilevel feedback queue
- Metrics: arrival, burst, completion, turnaround, waiting, response time, throughput
- Convoy effect, starvation, aging
- Time quantum selection effects
- Real-time scheduling: Rate Monotonic, Earliest Deadline First
- Multiprocessor scheduling (basic)
- Gantt chart numericals

#### Process synchronisation
- Critical section problem and its three requirements (mutual exclusion, progress, bounded waiting)
- Software solutions: Peterson, Dekker, Bakery
- Hardware support: Test-and-Set, Compare-and-Swap, disabling interrupts, atomic instructions
- Mutex vs binary semaphore vs counting semaphore; spinlock vs sleeping lock
- Monitors and condition variables
- Priority inversion and priority inheritance
- Classical problems: producer-consumer, readers-writers, dining philosophers, sleeping barber
- Race condition examples and fixes

#### Deadlock
- Four necessary conditions
- Resource allocation graph
- Prevention, avoidance (safe state, Banker's algorithm), detection, recovery
- Livelock vs deadlock vs starvation
- Resource-count formula numericals
- Ostrich algorithm

#### Inter-process communication
- Pipes, named pipes (FIFO), message queues, shared memory, semaphores, signals, sockets
- Message passing: send and receive, direct vs indirect, blocking vs non-blocking, buffering
- Shared memory vs message passing trade-offs
- Linux IPC APIs overview

#### Memory management
- Address binding, logical vs physical address, MMU
- Contiguous allocation: fixed and variable partitioning; first, best, worst, next fit
- Internal vs external fragmentation, compaction
- Paging, page table structures (multilevel, hashed, inverted), page table size numericals
- TLB, effective access time numericals
- Segmentation and segmented paging

#### Virtual memory
- Demand paging, page fault handling steps
- Page replacement: FIFO, LRU, Optimal, Second chance, Clock, LFU
- Belady's anomaly
- Thrashing, working set model, page fault frequency
- Copy-on-write, memory-mapped files, swapping, overcommit
- Memory allocators (buddy, slab) concept

#### File systems
- File attributes, operations, access methods
- Directory structures: single-level, two-level, tree, acyclic graph, general graph
- Allocation methods: contiguous, linked, FAT, indexed, multilevel indexed, combined (Unix inode)
- Free space management: bit vector, linked list, grouping, counting
- Inodes, hard links vs soft links, file descriptors
- FAT, NTFS, ext4, journaling
- Mounting, VFS layer, file permissions and ACLs

#### I/O and disk
- I/O hardware, polling, interrupt-driven I/O, DMA
- Buffering, caching, spooling
- Disk structure and access time
- Disk scheduling: FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK
- RAID levels

#### Linux and systems practice
- File and directory commands (ls, cp, mv, rm, find, locate, ln)
- Text processing: grep, awk, sed, cut, sort, uniq, wc, head, tail, tee, xargs
- Process tools: ps, top, htop, kill, nice, jobs, bg, fg, nohup
- Permissions: chmod, chown, umask, setuid, sticky bit
- Network tools: ping, traceroute, netstat, ss, tcpdump, curl, nslookup, dig, ifconfig/ip
- System tools: df, du, free, vmstat, iostat, lsof, strace, gdb, valgrind
- Shell scripting basics, pipes, redirection, environment variables, cron
- /proc filesystem, signals (SIGKILL, SIGTERM, SIGSEGV etc.), ELF format
- Static vs dynamic libraries (.a vs .so), LD_LIBRARY_PATH
- Device drivers, kernel modules, cgroups and namespaces (basic)

#### RTOS and embedded awareness
- Hard vs soft real-time, determinism, latency, jitter
- ISR design, interrupt latency, nested interrupts
- Memory-mapped I/O, bootloader, watchdog timer
- Task scheduling in RTOS, shared library trade-offs in RTOS
- Common RTOS names (FreeRTOS, VxWorks, QNX)

#### Security (basic)
- Protection and access control
- Buffer overflow, privilege escalation, authentication basics

---

### 3.4 Computer Networks

#### Models and basics
- OSI vs TCP/IP model, layer responsibilities, encapsulation and PDUs
- Network types (LAN, MAN, WAN), topologies
- Bandwidth, throughput, latency, jitter, propagation and transmission delay numericals
- Switching: circuit, packet, message

#### Physical and data link layer
- Transmission media, encoding, multiplexing
- Framing, error detection (parity, checksum, CRC), error correction (Hamming)
- Flow control: Stop-and-Wait, Go-Back-N, Selective Repeat; efficiency numericals
- MAC protocols: ALOHA, slotted ALOHA, CSMA, CSMA/CD, CSMA/CA
- Ethernet frame format, MAC addressing
- ARP and RARP
- Hub vs switch vs bridge vs router; switching table learning
- VLAN, trunking, Spanning Tree Protocol

#### Network layer
- IPv4 addressing, classful and classless (CIDR)
- Subnetting, VLSM, supernetting, route aggregation, numericals
- IPv6 (format, features, transition)
- NAT, PAT, private vs public addresses
- ICMP, ping, traceroute
- DHCP process (DORA)
- IP fragmentation and reassembly
- Routing: static vs dynamic, distance vector (RIP), link state (OSPF), path vector (BGP), count-to-infinity, split horizon
- Multicast, broadcast, anycast, IGMP
- MPLS (basic)

#### Transport layer
- TCP vs UDP, use cases, header fields
- Ports and sockets
- Connection setup (3-way handshake) and termination (4-way), TCP state diagram, TIME_WAIT
- Flow control (sliding window, receive window)
- Congestion control: slow start, congestion avoidance, fast retransmit, fast recovery, AIMD, Reno, Cubic
- Retransmission, RTT estimation, Nagle's algorithm
- SYN flood and other attacks
- QUIC (basic)

#### Application layer
- HTTP/1.1, HTTP/2, HTTP/3, methods, status codes, headers, persistent connections
- HTTPS, TLS handshake, certificates
- DNS: hierarchy, recursive vs iterative resolution, record types, caching
- Email: SMTP, POP3, IMAP
- FTP, SFTP, SSH, Telnet, SNMP
- WebSockets, REST, cookies and sessions
- Proxy, reverse proxy, CDN, load balancers (L4 vs L7)

#### Socket programming
- TCP and UDP client-server flow
- socket, bind, listen, accept, connect, send, recv, close
- Blocking vs non-blocking sockets, select, poll, epoll
- Concurrent server designs (fork, threads, event loop)
- Byte order (htons, ntohs)

#### Network security
- Firewalls, IDS/IPS, VPN, IPsec
- Symmetric vs asymmetric encryption, hashing, digital signatures, PKI
- DoS and DDoS, MITM, spoofing, ARP poisoning

#### Telecom awareness (Nokia-specific bonus)
- 4G LTE and 5G architecture basics (RAN, core network)
- SDN and NFV
- Optical networking and IP/MPLS basics
- Latency and reliability requirements in telecom systems

---

### 3.5 Other Areas

#### DBMS and SQL
- ER model, keys, normalisation (1NF to BCNF), functional dependencies
- SQL: joins, group by, having, subqueries, window functions
- Indexing (B+ tree), clustered vs non-clustered
- ACID, transactions, isolation levels, locking, deadlocks in databases
- SQL vs NoSQL basics

#### Python basics (embedded roles ask this)
- Data types, list/dict/set/tuple, comprehensions, generators, decorators
- OOPs in Python, exceptions, file handling, GIL

#### Aptitude (AMCAT-style)
- Quantitative: percentages, ratios, time-speed-distance, time and work, probability, permutations
- Logical reasoning and verbal ability basics

#### Projects
- Distributed task queue: architecture, queues, retries, dead-letter handling, failure scenarios, scaling, trade-offs
- JWT/session authentication: token flow, refresh, revocation, security threats
- Deep learning intrusion detection for IoT: dataset, model, metrics, limitations
- For every project prepare: problem, design decisions and the "why", alternatives, failure cases, what you would improve

#### HR
- Why Nokia, why this role, strengths and weaknesses
- Teamwork, conflict, failure and learning examples
- Relocation and shift flexibility, long-term goals

---

## 4. Study Method (Use for Every Topic)

1. Understand: learn the concept with an analogy, then the internals (why it works, not just what it is).
2. Explain aloud: speak a 2-minute explanation as if teaching someone. If you get stuck, you have not understood it yet.
3. Practise: 5 MCQs or output-prediction questions, or 2 coding problems.
4. Write a one-line cheat note (formula, trap, comparison, complexity).
5. Ask yourself: "What trap question would an interviewer ask on this?"

---

## 5. Day-by-Day Plan (4 Nights + 3 Days)

Assumption: about 10 to 12 study hours per day, plus 6 hours of sleep every night.

### Night 1 (about 4 hours): C++ Foundation
- Memory layout, stack vs heap, pointers vs references, const, static, sizeof and padding
- Constructors and destructors, copy vs move, Rule of 3/5, deep vs shallow copy
- Virtual functions, vtable and vptr, virtual destructor, object slicing, diamond problem
- Operator overloading

### Day 1
- Morning (4 hours): rest of C++ (smart pointers, RAII, move semantics, templates, lambdas, exceptions, STL internals, thread/mutex basics)
- Afternoon (4 hours): OS part 1 (process vs thread, fork/exec, zombie/orphan, PCB, context switch, system calls, all CPU scheduling algorithms with numericals)
- Evening (2 hours): 20 C++ output-prediction and MCQ questions

### Night 2 (about 4 hours): OS Synchronisation and Deadlock
- Race condition, critical section, Peterson, Test-and-Set, CAS, mutex vs semaphore vs spinlock vs monitor
- Producer-consumer, readers-writers, dining philosophers (write the code yourself)
- Deadlock conditions, Banker's algorithm, prevention, resource-count numericals, priority inversion

### Day 2
- Morning (4 hours): OS part 2 (paging, TLB, EAT, virtual memory, page replacement, thrashing, file allocation, disk scheduling, IPC, RTOS and interrupts, Linux commands)
- Afternoon (4 hours): Networking part 1 (OSI vs TCP/IP, IP addressing, subnetting and CIDR numericals, ARP, NAT, DHCP, ICMP, routing protocols)
- Evening (2 hours): mixed OS numericals (scheduling, paging, deadlock)

### Night 3 (about 4 hours): Networking Part 2
- TCP vs UDP, handshake, flow and congestion control, DNS resolution, HTTP/HTTPS/TLS, socket API, select/epoll
- Data link layer: CRC, sliding window protocols, CSMA/CD, switch/hub/VLAN

### Day 3
- Morning (4 hours): DSA revision (not new learning)
  - Write the one-page complexity table
  - Linked list (cycle, reverse, circular and doubly), sliding window, hashing and anagrams, sieve and perfect-square number theory
  - Write 1 to 2 solutions per pattern from scratch without looking
  - Revise Fenwick Tree, Segment Tree, DSU, Dijkstra, Kruskal, KMP, Rabin-Karp
- Afternoon (3 hours): projects deep dive (design decisions, trade-offs, failure cases), DBMS in 1 hour, SOLID
- Evening (2 hours): HR answers, written then spoken aloud

### Night 4 (light, 2 to 3 hours, then sleep)
- Read your cheat notes
- Rapid-fire mock interview: C++, OS, CN, DSA, project, HR
- Revisit only weak topics
- Sleep by 11 to 12 PM; a rested mind performs better than extra cramming

---

## 6. Priority Order (If Time Runs Short)

Never skip:
1. C++ OOPs, memory, virtual functions, STL, modern C++
2. OS: synchronisation, scheduling, deadlock, memory management, IPC, Linux
3. DSA (easy to medium): linked lists, arrays and strings, trees, graph basics, sliding window, complexities
4. Networking: OSI, TCP/IP, IP addressing and subnetting, TCP vs UDP, DNS, HTTP
5. Project deep dive and HR

Cut in this order if needed:
1. Telecom bonus topics
2. Windows-specific trivia
3. Python
4. DBMS
5. Advanced DSA (digit DP, suffix arrays, etc.)

---

## 7. Final Checklist Before the Interview

- [ ] Can explain vtable and virtual destructor from scratch
- [ ] Can write Rule of 5 class, smart pointer usage and a thread-safe singleton
- [ ] Can explain mutex vs semaphore with a real example and code the producer-consumer problem
- [ ] Can solve scheduling, paging and deadlock numericals quickly
- [ ] Can explain what happens when you type a URL in the browser (DNS, TCP, TLS, HTTP)
- [ ] Can do subnetting numericals in under 2 minutes
- [ ] Can write linked-list cycle detection and removal, Dijkstra, DSU, and KMP without help
- [ ] Can explain every resume project and its trade-offs in 3 minutes
- [ ] HR answers rehearsed aloud
- [ ] Slept well the night before
