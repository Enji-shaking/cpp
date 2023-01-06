A concept easy to be forgotten. 
It talks about we have a pointer to a base class that defines virtual functions, while that pointer actually point to a derived class.
When we call the virtual functions, it will call the implementation in the derived classes.

At the same time, dereferencing the base class pointer will still gets us the base class. We could witness this through the size of the classes.

``` C++

#include <iostream>
#include <type_traits>
using namespace std;

class A{
    public:
    virtual void print(){
        cout << "A" << endl;
    }
    void print2(){
        cout << "A2" << endl;
    }
};
class B: public A{
    public:
    virtual void print(){
        cout << "B" << endl;
    }
    void print2(){
        cout << "B2" << endl;
    }
};


int main()
{
    A* a = new B();
    a->print();
    a->print2();
    
    A a1;
    B b1;
    cout << sizeof(*a) << endl;
    cout << sizeof(a1) << endl;
    cout << sizeof(b1) << endl;
    return 0;
}
/*
B
A2
8
8
16
*/
```