#include<iostream>
using namespace std;

class Student{
public:
    int id;
    int age;
    string name;
    int noOfSub;

    Student(string name){
        this->name = name;
        cout << this->name <<" Constructor of Class" << endl;
    }

    void speak(){
        cout << "Student can speak" << endl;
    }

    ~Student(){
        cout << this->name << " Destructor of Class" << endl;
    }
};

int main(){
    Student A("ABC");
    Student X("XYZ");
    Student L("LMN");

    // Parameterized Constructor (Student(string name)) & this pointer:
    // When you create Student A("ABC") and Student X("XYZ"), you pass a string to the constructor. 
    // The this pointer ensures that the member variable name belongs to the specific object instance currently being initialized. 
    
    // Stack Destruction Order (LIFO):
    // Objects A and X are created on the stack inside main(). 
    // Stack memory follows a LIFO (Last In, First Out) rule, meaning the last object created is the first one destroyed 
    // when it goes out of scope.

    // L was created last, so its desctructor run first(LMN Destructor of Class).
    // X was created second last, so its destructor runs second (XYZ Destructor of Class).
    // A was created first, so its destructor runs last (ABC Destructor of Class).

    return 0;
}