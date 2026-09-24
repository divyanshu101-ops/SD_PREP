function isEven(n) {
    if (n % 2 === 0) {
        return true;
    }
    return false;
}

console.log(isEven(3));
console.log(isEven(10));
console.log(isEven(0));
console.log("---------------------------------");

function greet(name) {
    return `Hello ${name}`;
}

console.log(greet("Divyanshu"))
console.log(greet("Suhani"))

console.log("---------------------------------");

function max(a, b) {
    if (a < b) return b;

    return a;
}

console.log(max(4, 3));
console.log(max(926, 2372));


console.log("---------------------------------");


function countVowels(s) {
    const n = s.length
    const vowels = new Set(['a', 'e', 'i', 'o', 'u']);
    let count = 0;
    for (let i = 0; i < n; i++) {
        if (vowels.has(s[i])) {
            count++;
        }
    }

    return count;
}


console.log(countVowels("hello world")); // Output: 3
console.log(countVowels("JavaScript"));  // Output: 3

console.log("---------------------------------");

function sumArray(num) {
    const n = num.length;
    let sum = 0;

    for (let i = 0; i < n; i++) {
        sum += num[i];
    }

    return sum;
}

let sum = sumArray([23, 32]);
console.log(sum);

console.log("------------------------------");

// Q1
console.log(hello());
function hello() { return "hi"; }

// Q2
// console.log(hey());
// var hey = function () { return "hey"; }; // typeError 

// Q3
function test() { const a = 5; }
console.log(test());

// Q4
function f(x = 10) { return x; }
console.log(f(null), f(undefined), f());