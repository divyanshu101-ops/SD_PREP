console.log(greeting);
var greeting = "Namaste JavaScript";

function test() {
    console.log(score);
    var score = 100;
}
test();

// my answer
// undefined
// 100

// correct answer
// undefined
// undefined -> Because of the scope of the variable,
// during the time of printing the 'score' variable inside the test function,
// it was not yet initialized, so it is undefined.
