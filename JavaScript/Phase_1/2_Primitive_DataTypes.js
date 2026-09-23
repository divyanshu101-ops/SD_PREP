// Primitive Data Types:
// 1. String
// 2. Number
// 3. BigInt
// 4. Boolean
// 5. undefined
// 6. null
// 7. Symbol


/// ==========================================
// 1. PRIMITIVE DATA TYPES
// ==========================================

// Number
let age = 21;
let cgpa = 7.48;

console.log("Age:", age);
console.log("CGPA:", cgpa);

// Logic
if (cgpa >= 7) {
    console.log("Eligible for interview");
}


// String
let name = "Divyanshu";
let role = "Backend Developer";

console.log("Name:", name);
console.log("Role:", role);

// String concatenation
console.log(name + " is preparing for " + role);


// Boolean
let isStudent = true;
let hasBacklog = false;

if (isStudent && !hasBacklog) {
    console.log("Student is eligible");
}


// Undefined
let internship;

console.log("Internship:", internship);

if (internship === undefined) {
    console.log("Internship is not assigned yet");
}


// Null
let selectedCompany = null;

console.log("Selected Company:", selectedCompany);

if (selectedCompany === null) {
    console.log("No company selected");
}


// Symbol
let id = Symbol("id");

console.log("Symbol:", id);


// BigInt
let bigNumber = 9007199254740991n;

console.log("Big Number:", bigNumber);

let anotherBigNumber = 10n;

console.log("Addition:", bigNumber + anotherBigNumber);
