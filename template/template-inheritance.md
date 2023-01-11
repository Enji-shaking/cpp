### Typical bugs

#### Inheritance stack overflow
```C++
struct base {
    base() {}
    template <typename T>
    base(T x) {} 
};
struct derived : base
{
    derived() {}
    derived(const derived& that): base(that) {} 
};
derived d1;
derived d2 = d1;
```
The assignment `d2 = d1` causes a stack overflow.

Reasoning:
* it invokes copy constructor to construct d2
* in the constructor, it passes d1 to the base constructor because d1 matches T = derived
* in the base constructor, it's making a copy to d1, which will invoke copy constructor again
* Thus, stack overflow