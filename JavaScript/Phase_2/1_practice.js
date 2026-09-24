function sum(a, b) {
    return a + b;
}

function factorial(n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

function isPalindrome(s) {
    let i = 0;
    let j = s.length - 1;

    while (i < j) {
        if (s[i] !== s[j]) {
            return false;
        }

        i++;
        j--;
    }

    return true;
}

console.log(sum(23, 324));

console.log(factorial(9));

console.log(isPalindrome("madam"));

console.log(isPalindrome("Divya"));


console.log(1 + 1 + 1 + 1 + 1);