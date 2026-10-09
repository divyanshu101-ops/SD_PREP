#include<iostream>
using namespace std;

class Car {
    // In C++ By Default everything in class is Private
public: 
    int modelNo;
    string companyName;
    int noOfGears;

    // Default Constructor
    Car() {
        cout << "Default Constructor: Object of Car Created" << endl;
    }

    // Parameterized Constructor
    // In modern C++ this way we can assign values to the variables of class
    Car(int m, string c, int n) : modelNo(m), companyName(c), noOfGears(n) {
        cout << "Parameterized Constructor called for: " << companyName << endl;
    }

    // Copy Constructor
    Car(const Car &c) {
        // 1. Dot (.) operator: Used with direct objects (e.g., c2.modelNo)
        // 2. Arrow (->) operator: Used with object pointers (e.g., ptr->modelNo)
        this->modelNo = c.modelNo;          
        this->companyName = c.companyName;
        this->noOfGears = c.noOfGears;
        cout << "Copy Constructor Called for: " << this->companyName << endl;
    }
}; // <--- Yahan semicolon (;) zaroori hai

int main() {
    Car c1;                           // Default Constructor chalega
    Car c2(2026, "Tesla", 1);         // Parameterized Constructor chalega
    Car c3 = c2;                      // Copy Constructor chalega (c2 ki values c3 mein copy hongi)
    
    return 0;
}