#include <iostream>
#include <string>
using namespace std;

// 1. Classes are defined in machine code at compile-time as a blueprint/structure.
class Engine {
public:
    int horsepower;

    Engine(int hp) {
        horsepower = hp;
        cout << "Engine created with " << horsepower << " HP." << endl;
    }

    ~Engine() {
        cout << "Engine destroyed." << endl;
    }
};

class Car {
private:
    string modelName;
    Engine *enginePtr; // Pointer to another class object

public:
    // Parameterized Constructor
    Car(string name, int hp) {
        modelName = name;
        // Creating a dynamic object for the Engine class inside Car
        enginePtr = new Engine(hp); 
        cout << "Car " << modelName << " constructed." << endl;
    }

    void showCarDetails() {
        // 5. Dynamic allocation returns a memory address stored in a pointer, 
        // accessed using the arrow (->) operator.
        cout << "Car Model: " << modelName << ", Horsepower: " << enginePtr->horsepower << endl;
    }

    // Destructor to clean up dynamically allocated memory
    ~Car() {
        delete enginePtr; // Freeing heap memory to prevent memory leaks
        cout << "Car " << modelName << " destroyed." << endl;
    }
};

int main() {
    // 2. Objects are created and allocated memory in RAM at run-time when the program runs.
    
    cout << "--- Starting Static Allocation ---" << endl;
    // 3. Static allocation (Stack memory)
    // 4. Static allocation gives a direct object where properties and behaviors are accessed directly.
    Car myStaticCar("Tesla Model 3", 450);
    myStaticCar.showCarDetails(); // Accessed directly using dot (.) operator

    cout << "\n--- Starting Dynamic Allocation ---" << endl;
    // 3. Dynamic allocation (Heap memory)
    // 5. Dynamic allocation returns a memory address stored in a pointer
    Car *myDynamicCar = new Car("Porsche 911", 500);
    
    // Accessed using the arrow (->) operator because myDynamicCar is a pointer
    myDynamicCar->showCarDetails(); 

    // Explicitly deleting the dynamically allocated object from Heap memory
    delete myDynamicCar; 

    cout << "\n--- End of Program ---" << endl;
    return 0;
}