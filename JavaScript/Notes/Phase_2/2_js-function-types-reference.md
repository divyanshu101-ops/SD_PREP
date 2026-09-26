# JavaScript Functions: Complete Reference Guide & Types

A complete reference to every major "flavor" of function in JavaScript — what it is, why it exists, its quirks, and code examples with their exact output.

---

## 1. Function Declaration (Named Statement)

### What it is
The most standard way to create a function. It always has a name, starts with the `function` keyword, and is not assigned to a variable.

```js
function add(a, b) {
  return a + b;
}
```

### Key trait: Hoisting
Function declarations are hoisted **completely** — the whole function body is stored in memory before any code runs. This means you can call it *before* the line where it's written.

```js
console.log(multiply(4, 5));   // Output: 20

function multiply(a, b) {
  return a * b;
}
```

**Output:**
```
20
```

### When to use it
General-purpose, reusable functions that you want available anywhere in the file, regardless of order.

---

## 2. Function Expression (Anonymous Function assigned to a variable)

### What it is
An anonymous (nameless) function stored inside a variable. The function itself has no name — the variable is what you use to call it.

```js
const add = function (a, b) {
  return a + b;
};

console.log(add(3, 4));
```

**Output:**
```
7
```

### Key trait: Not hoisted (like a declaration)
```js
console.log(square(5));   // Throws error

const square = function (n) {
  return n * n;
};
```

**Output:**
```
ReferenceError: Cannot access 'square' before initialization
```

Because `square` is declared with `const`, it sits in the **Temporal Dead Zone (TDZ)** until its line executes.

### When to use it
When you want to pass a function around as a value, create it conditionally, or use it as a callback.

---

## 3. Named Function Expression

### What it is
Same as a function expression, but the function itself carries an internal name. That internal name is only visible **inside the function's own body** — useful for recursion or clearer stack traces when debugging.

```js
const factorial = function fact(n) {
  if (n <= 1) return 1;
  return n * fact(n - 1);   // "fact" refers to itself internally
};

console.log(factorial(5));
```

**Output:**
```
120
```

Trying to use the internal name outside fails:

```js
console.log(fact);   // Throws error
```

**Output:**
```
ReferenceError: fact is not defined
```

### When to use it
Recursive function expressions, or when you want a meaningful name to show up in stack traces during debugging.

---

## 4. Arrow Function (ES6 Concise Syntax)

### What it is
A shorter syntax introduced in ES6. No `function` keyword needed.

```js
const add = (a, b) => a + b;
console.log(add(2, 3));
```

**Output:**
```
5
```

### Implicit return
If the function body is a single expression with no `{ }`, it's returned automatically.

```js
const square = n => n * n;       // single param, no parentheses needed
console.log(square(6));
```

**Output:**
```
36
```

With `{ }`, you must use `return` explicitly:

```js
const square2 = n => {
  return n * n;
};
```

### Key trait 1: No own `this`
Arrow functions don't create their own `this` — they use `this` from the surrounding (lexical) scope. This is why they're extremely useful inside callbacks.

```js
function Timer() {
  this.seconds = 0;

  setInterval(() => {
    this.seconds++;             // "this" refers to the Timer object
    console.log(this.seconds);
  }, 1000);
}

new Timer();
```

**Output (once per second):**
```
1
2
3
```

Compare with a regular function, which loses `this`:

```js
function BrokenTimer() {
  this.seconds = 0;

  setInterval(function () {
    this.seconds++;             // "this" is undefined or global object here
    console.log(this.seconds);
  }, 1000);
}

new BrokenTimer();
```

**Output:**
```
NaN
NaN
NaN
```

### Key trait 2: No `arguments` object
```js
const showArgs = () => {
  console.log(arguments);
};
showArgs(1, 2, 3);
```

**Output:**
```
ReferenceError: arguments is not defined
```

### Key trait 3: Cannot be used as a constructor
```js
const Person = (name) => {
  this.name = name;
};

const p = new Person("Divyanshu");
```

**Output:**
```
TypeError: Person is not a constructor
```

### When to use it
Short callbacks, array methods (`map`, `filter`, `reduce`), and anywhere you need to preserve the outer `this`.

---

## 5. Immediately Invoked Function Expression (IIFE)

### What it is
A function that runs the instant it's defined — it wraps itself in parentheses and calls itself immediately.

```js
(function () {
  console.log("I ran immediately!");
})();
```

**Output:**
```
I ran immediately!
```

With arrow function syntax:

```js
(() => {
  console.log("Arrow IIFE ran!");
})();
```

**Output:**
```
Arrow IIFE ran!
```

### Key trait: Avoids polluting the global scope
Anything declared inside stays private.

```js
(function () {
  const secret = "hidden";
  console.log(secret);
})();

console.log(secret);   // Not accessible outside
```

**Output:**
```
hidden
ReferenceError: secret is not defined
```

### IIFE with parameters
```js
(function (name) {
  console.log(`Hello, ${name}`);
})("Divyanshu");
```

**Output:**
```
Hello, Divyanshu
```

### When to use it
Module pattern (before ES6 modules existed), setup code that should run once, or keeping variables out of the global scope.

---

## 6. Callback Function

### What it is
A function passed as an **argument** to another function, to be executed later — either synchronously or asynchronously.

### Synchronous callback
```js
function greet(name, callback) {
  console.log("Hi " + name);
  callback();
}

greet("Divyanshu", function () {
  console.log("Callback executed!");
});
```

**Output:**
```
Hi Divyanshu
Callback executed!
```

### Asynchronous callback
```js
console.log("Start");

setTimeout(function () {
  console.log("This runs later");
}, 2000);

console.log("End");
```

**Output:**
```
Start
End
This runs later
```
(`"This runs later"` prints after a 2-second delay, because `setTimeout` is asynchronous.)

### Callback with array methods
```js
[1, 2, 3].forEach(function (num) {
  console.log(num * 2);
});
```

**Output:**
```
2
4
6
```

### The problem: Callback Hell
```js
loginUser(function (user) {
  getOrders(user, function (orders) {
    getOrderDetails(orders, function (details) {
      console.log(details);
    });
  });
});
```
Deep nesting like this is why Promises and `async/await` (covered in later phases) exist.

### When to use it
Whenever you need "do this task, and once it's done, run this other code" — event handlers, array iteration, and async operations.

---

## 7. Higher-Order Function (HOF)

### What it is
A function that either:
1. Takes another function as an argument, **or**
2. Returns a function.

### Type 1: Takes a function as an argument
```js
function operate(a, b, operation) {
  return operation(a, b);
}

const sum = (x, y) => x + y;
console.log(operate(5, 3, sum));
```

**Output:**
```
8
```

Built-in HOFs you already use: `map`, `filter`, `reduce`, `sort`, `forEach`:

```js
const nums = [1, 2, 3, 4, 5];

const doubled = nums.map(n => n * 2);
const evens = nums.filter(n => n % 2 === 0);
const total = nums.reduce((acc, n) => acc + n, 0);

console.log(doubled);
console.log(evens);
console.log(total);
```

**Output:**
```
[ 2, 4, 6, 8, 10 ]
[ 2, 4 ]
15
```

### Type 2: Returns a function
```js
function multiplier(factor) {
  return function (num) {
    return num * factor;
  };
}

const double = multiplier(2);
const triple = multiplier(3);

console.log(double(5));
console.log(triple(5));
```

**Output:**
```
10
15
```

### When to use it
Writing generic, reusable logic — this is the foundation for closures, currying, and functional programming patterns used heavily in backend code.

---

## 8. Constructor Function

### What it is
A blueprint for creating multiple similar objects. Called with the `new` keyword, and by convention its name starts with a capital letter.

```js
function Person(name, age) {
  this.name = name;
  this.age = age;
}

Person.prototype.greet = function () {
  console.log(`Hi, I'm ${this.name} and I'm ${this.age} years old.`);
};

const p1 = new Person("Divyanshu", 21);
const p2 = new Person("Rahul", 22);

p1.greet();
p2.greet();
```

**Output:**
```
Hi, I'm Divyanshu and I'm 21 years old.
Hi, I'm Rahul and I'm 22 years old.
```

### What `new` actually does
1. Creates a new empty object.
2. Sets `this` to point to that object.
3. Links the object's prototype to `Person.prototype`.
4. Returns the object automatically (unless the function explicitly returns another object).

### Forgetting `new` — a classic bug
```js
const p3 = Person("Amit", 25);   // no "new"
console.log(p3);
console.log(name);               // leaked to global scope in non-strict mode
```

**Output (non-strict mode):**
```
undefined
Amit
```
Without `new`, `this` refers to the global object (or `undefined` in strict mode), so the properties leak or the code throws.

### When to use it
Older pattern for creating multiple objects with shared behavior — largely replaced today by ES6 `class` syntax, but understanding it explains how `class` works under the hood.

---

## 9. Generator Function (`function*`)

### What it is
A special function that can **pause** its execution using `yield` and **resume** later from exactly where it left off. It returns an iterator, not a normal value.

```js
function* countUp() {
  console.log("Start");
  yield 1;
  console.log("Resumed after first yield");
  yield 2;
  console.log("Resumed after second yield");
  yield 3;
  console.log("Done");
}

const gen = countUp();

console.log(gen.next());
console.log(gen.next());
console.log(gen.next());
console.log(gen.next());
```

**Output:**
```
Start
{ value: 1, done: false }
Resumed after first yield
{ value: 2, done: false }
Resumed after second yield
{ value: 3, done: false }
Done
{ value: undefined, done: true }
```

### Using a generator with a `for...of` loop
```js
function* colors() {
  yield "red";
  yield "green";
  yield "blue";
}

for (const color of colors()) {
  console.log(color);
}
```

**Output:**
```
red
green
blue
```

### Infinite sequence example
```js
function* idGenerator() {
  let id = 1;
  while (true) {
    yield id++;
  }
}

const ids = idGenerator();
console.log(ids.next().value);
console.log(ids.next().value);
console.log(ids.next().value);
```

**Output:**
```
1
2
3
```

### When to use it
Lazy evaluation, custom iterators, infinite sequences, and (historically) managing async flows before `async/await` existed.

---

## Summary Table

| # | Type | Hoisted? | Has own `this`? | Can pause execution? | Typical use |
|---|---|---|---|---|---|
| 1 | Function Declaration | Yes | Yes | No | General reusable functions |
| 2 | Function Expression | No | Yes | No | Values, conditional creation |
| 3 | Named Function Expression | No | Yes | No | Recursion, debugging |
| 4 | Arrow Function | No | No (lexical) | No | Callbacks, array methods |
| 5 | IIFE | N/A (runs instantly) | Yes | No | Scope isolation, one-time setup |
| 6 | Callback Function | Depends on form | Depends on form | No | Async/sync task chaining |
| 7 | Higher-Order Function | Depends on form | Depends on form | No | Functional composition |
| 8 | Constructor Function | No | Yes (new instance) | No | Creating multiple similar objects |
| 9 | Generator Function | No | Yes | Yes | Iterators, lazy sequences |

---

## Practice Problems

1. Write a **function declaration** `isPrime(n)` and call it before its definition to confirm hoisting works.
2. Write a **named function expression** to compute Fibonacci recursively.
3. Convert a regular `function` callback inside `setTimeout` into an **arrow function**, and explain why `this` behaves differently.
4. Write an **IIFE** that creates a private counter (`increment()`, `getCount()`) not accessible from outside.
5. Write a **higher-order function** `applyDiscount(price, discountFn)` where `discountFn` calculates the discount.
6. Write a **constructor function** `Car(brand, model)` with a `.describe()` method on its prototype.
7. Write a **generator function** that yields the first 5 even numbers.

Try predicting the output of each before running it — that's the fastest way to internalize hoisting, `this`, and execution order.
