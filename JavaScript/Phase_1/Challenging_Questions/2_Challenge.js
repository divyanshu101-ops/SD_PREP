let engine = "V8";
{
    console.log(engine);
    let engine = "Node.js";
}

// my answer

// ReferenceError -> Because of the scope of the variable,
// during the time of printing the 'engine' variable inside the block,
// it was not yet initialized, so it is ReferenceError.