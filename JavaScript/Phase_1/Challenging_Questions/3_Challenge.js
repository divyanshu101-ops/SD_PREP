let data = 42;
console.log(typeof data); // Number

data = null;
console.log(typeof data); // object

let unassigned;
console.log(typeof unassigned); // undefined 

// (var) function-scoped hota hai isliye block
// ke bahar bhi access ya change ho sakta hai (jo ki bugs laata hai), jabki (let) aur
// (const) strict block-scoped hote hain. Aur (const) mein toh re-assignment
// ka koi scene hi nahi hai, wo ek baar fix ho gaya toh ho gaya.