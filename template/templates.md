### policy based class design

<small> From modern C++ design Chapter 1 </small>

We don't want to combine everything under one umbrella. It influences portability. 

We also cannot implement multiple smaller classes, such as SingleThreadedSmartPtr, MultiThreadedSmartPtr, RefCountedSmartPtr, RefLinkedSmartPtr, and so on.

We might use multiple inheritance to solve the problem. For example, a TemporarySecretary could inherit from Temporary and Secretary. 
    
* The base classes do not have enough type information to carry out their tasks.  (?Questioning?)




```C++
#include <iostream>
using namespace std;
int main(){
    cout << "happy new year";
}
```

#### basic policy example
You can implement several policy classes for a given policy. They all must respect the interface as defined by the policy. The user then chooses which policy class to use in larger structures, as you will see.


```C++
#include <iostream>
using namespace std;

template <typename T>
struct OpNewCreator 
{ 
    static T* Create() { 
        return new T; 
    } 
};

struct Widget{
    void print(){
        cout << "print from widget" << endl;
    }
};

template <typename CreationPolicy> 
struct WidgetManager : public CreationPolicy
{ 
 void DoSomething() 
 {
    Widget* ptr = CreationPolicy::Create();
    ptr->print(); 
 } 
};

int main(){
    WidgetManager<OpNewCreator<Widget>> op;
    op.DoSomething();
}
```
Here, WidgetManager class is our host class. It has one policy requirement, that the creating policy classes need to implement `Create()` and the created object needs to have `print()` implemented. As of how to create or what does print() do, it's not the manager's concern. 

However, this has a problem. As a WidgetManager, it knows it manage Widgets. We should not ask the client code to specify Widget in this line
`WidgetManager<OpNewCreator<Widget>> op;`

Writing template code like this achieves what we want

```C++
...
template <template <typename bluhbluh> typename CreationPolicy>
struct WidgetManager : public  CreationPolicy<Widget>
{ 
 void DoSomething() 
 {
    Widget* ptr = CreationPolicy<Widget>::Create();
    ptr->print(); 
 } 
};
int main(){
    WidgetManager<OpNewCreator> op;
    op.DoSomething();
}
```
the inside `typename bluhbluh` is simply a placeholder. It tells the compiler that the template type for CreationPolicy needs to have one parameter itself.

* typename is like `class` in c++. In normal way, `template <typename A> ` can be translated as: A template with a class named A.
* Like `class` can be described by `const`, the `typename` can also be described. Here, `template <typename bluhbluh>` is describing the type it really is.



### Partial specification
Typically, in a partial specialization of a class template, you specify only some of the template arguments and leave the other ones generic.

For example, for the following definition

`template <class T, class U> class SmartPtr { ... };`

we can specialize `SmartPtr<T, U>` for Widget and any other type using the following syntax: 

`template <class U> class SmartPtr<Widget, U> { ... };`

Here, we manually specified T as Widget.

<details>
<summary>
An example about partial initialization, together with Policy, I guess. Different classes inherit different policies, differentiating by the partially specified type.
</summary>

``` C++
template <typename T, typename U>
struct testPrinter 
{ 
    void print(){
        cout << "T, U" << endl;
    }
};

template <typename T>
struct testPrinter<T, char>
{ 
    void print(){
        cout << "T, char" << endl;
    }
};

template <>
struct testPrinter<int, int>
{ 
    void print(){
        cout << "int int" << endl;
    }
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer1 : public PrinterPolicy<int, char>
{ 
    typedef PrinterPolicy<int, char> type;
    void DoSomething() 
    {
        type::print();
    } 
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer2 : public PrinterPolicy<int, int>
{ 
    typedef PrinterPolicy<int, int> type;
    void DoSomething() 
    {
        type::print();
    } 
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer3 : public PrinterPolicy<string, string>
{ 
    typedef PrinterPolicy<string, string> type;
    void DoSomething() 
    {
        type::print();
    } 
};

int main(){
    printerContainer1<testPrinter> t1;
    printerContainer2<testPrinter> t2;
    printerContainer3<testPrinter> t3;
    t1.DoSomething();
    t2.DoSomething();
    t3.DoSomething();
}
```
</details>

An example of specifying variables partially specialized classes

* To initialize a class with static member variable

```C++
#include <iostream>
// #include <type_traits>
using namespace std;

template <class T>
class Widget 
{ 
    public:
    void Fun() { 
        cout << "original fun" << endl;
    } 
    static int a;
}; 

template<class T>
int Widget<T>::a = 4;

int main()
{
    Widget<char> w;
    w.Fun();
    cout << w.a << endl;

    return 0;
}
// original fun
// 4
```

* You cannot specialize static variables on some specialized version. With the code above, adding these lines specialize value a in an int Widget.

```C++
template<>
int Widget<int>::a = 5;
int main()
{
    Widget<char> w;
    w.Fun();
    Widget<int> w2;
    cout << w.a << endl;
    cout << w2.a << endl;

    return 0;
}
/*
original fun
4
5
*/
```

* Specialization of member functions does not scale. No: We cannot specialize part of the class and its function
* You have to specify all possibilities

``` C++
template <class T> class Widget 
{ 
 void Fun() { .. generic implementation ... } 
}; 
// OK: specialization of a member function of Widget 
template <> Widget<char>::Fun() 
{ 
 ... specialized implementation ... 
} 

template <class T, class U> class Gadget 
{ 
 void Fun() { .. generic implementation ... } 
}; 
// Error! Cannot partially specialize a member class of Gadget 
template <class U> void Gadget<char, U>::Fun() 
{ 
 ... specialized implementation ... 
}
// Okay!
template <> void Gadget<char, int>::Fun() 
{ 
//  ... specialized implementation ... 
} 
```

#### Templated Functions
Function partial specialization is not allowed yet as per the standard.
Only overloading is possible.

* partial template specialization does not apply to function
* you can totally specialize member functions of a class template, you cannot partially specialize member functions.
* You cannot partially specialize namespace-level (nonmember) template functions.
* The closest thing to partial specialization for namespace-level template functions is overloading.
  * you have fine-grained specialization abilities only for the function parameter
  * ```C++
    template <class T, class U> T Fun(U obj); // primary template 
    template <class U> void Fun<void, U>(U obj); // illegal partial specialization 
    template <class T> T Fun (Window obj); // legal (overloading) ```

<details>
<summary>This is an **incorrect** example where trying to do a specialization</summary>

```C++
template <typename T, typename U>
void dummy(){
    cout << "dummy" << endl;
}

template <int, typename U>
void dummy(){
    cout << "dummy int" << endl;
}

template <int, int>
void dummy(){
    cout << "dummy int int" << endl;
}

template<int, char>
void dummy(){
    cout << "dummy int char" << endl;
}
int main()
{
  
    dummy<double, double>();
    dummy<int, char>();
    dummy<int, string>();
    dummy<int, int>();
    
    return 0;
}
/*
dummy
dummy
dummy
dummy
*/
```
The results are all dummy, which shows that the functions are not separated. We have to specify it through the use of arguments, overloading. 
</details>


<details>

<summary>
For the following example, note that we **should not** use the function via specializing the template arguments. We should simply use the arguments in the function argument section
</summary>

```C++
template <typename U>
void dummy(int, U){
    cout << "dummy int" << endl;
}

template <>
void dummy(int, int){
    cout << "dummy int int" << endl;
}

template<>
void dummy(int, char){
    cout << "dummy int char" << endl;
}

template <typename T, typename U>
void dummy(T, U){
    cout << "dummy normal" << endl;
}

int main()
{
    // dummy normal
    dummy<>(double(), double());
    // dummy int char
    dummy<>(int(), int());
    // dummy normal
    // This is dummy normal because we provided 2 arguments
    dummy<int, int>(int(), int());
    // dummy int char
    dummy(int(), char());
    // dummy int char
    dummy(3, '4');
    // dummy int
    dummy(3, "44");
    // dummy int 
    dummy<>(3, "44");
    // This is dummy normal because we provided 2 arguments
    // dummy normal
    dummy<int, string>(3, "44");
    
    return 0;
}
```

</details>

### Some general techniques
#### Compile Time Assertions
(Modern C++ design Chapter 2.1)
Run time assertion example
```C++
template <class To, class From> 
To safe_reinterpret_cast(From from) 
{ 
 assert(sizeof(From) <= sizeof(To)); 
 return reinterpret_cast<To>(from); 
} 

int i = ...; 
char* p = safe_reinterpret_cast<char*>(i);
```
You specify the To template argument explicitly, and the compiler deduces the From template argument from i's type.

One **Old** and **not recommended** way

``` C++
#define STATIC_CHECK(expr) { char unnamed[(expr) ? 1 : 0]; } 

template <class To, class From> 
To safe_reinterpret_cast(From from) 
{ 
 STATIC_CHECK(sizeof(From) <= sizeof(To)); 
 return reinterpret_cast<To>(from); 
} 
... 
void* somePointer = ...; 
char c = safe_reinterpret_cast<char>(somePointer); 
```

This works but the error message is not okay:
"Cannot create array of size zero" does not suggest "Type char is too narrow to hold a pointer."
...
Wait
...
I think **static assert** is something we could use? (since C++11)
```C++
#include <iostream>
using namespace std;

int main()
{
    static_assert(3==3, "3 == 3");
    static_assert(3==4, "3 != 4");
    static_assert(3==5, "3 != 5");
    static_assert(3==6);
    cout << "hi" << endl;

    return 0;
}

```

#### Local Classes
You can define classes right inside functions
``` C++
void Fun() 
{ 
 class Local 
 { 
 ... member variables ... 
 ... member function definitions ... 
 }; 
 ... code using Local ... 
}
```
local classes **cannot** define static member variables and **cannot** access nonstatic local variables
You can use them in template functions
 &ensp;&ensp;&ensp; The class can use the *template parameters* of the enclosing function.

<details>

<summary>This example shows how a local class is used to implement an interface (inheriting)</summary>

The Local class inherits the `Interface` class and implements a function specified by `Interface`. Whoever calls MakeAdapter will return a pointer to an object which implements the Interface

But why are we doing this? 
* We can probably simply implement this outside of the function. 
* The benefit is that it's **final**, that outside users cannot derive from the class, which is **hidden** in a function. 
```C++
class Interface 
{ 
public: 
    virtual void Fun() = 0;
    ... 
}; 
template <class T, class P> 
Interface* MakeAdapter(const T& obj, const P& arg) 
{ 
    class Local : public Interface 
    { 
    public: 
        Local(const T& obj, const P& arg) 
        : obj_(obj), arg_(arg) {} 
        virtual void Fun() 
        { 
            obj_.Call(arg_); 
        } 
    private: 
        T obj_; 
        P arg_; 
    }; 
    return new Local(obj, arg); 
} 
```
</details>

#### Mapping Integral Constants to Types

For each integral number, you have a different type
```C++
template <int v> 
struct Int2Type 
{ 
 enum { value = v }; 
}; 
```
Use this when we want to **Call one of several different functions depending on a compile-time constant**

Runtime dispatch utilizes if-els.

```C++
template <typename T, bool isPolymorphic> 
class NiftyContainer 
{ 
private: 
    void DoSomething(T* pObj, Int2Type<true>) 
    { 
        T* pNewObj = pObj->Clone(); 
        ... polymorphic algorithm ... 
    } 
    void DoSomething(T* pObj, Int2Type<false>) 
    { 
        T* pNewObj = new T(*pObj); 
        ... nonpolymorphic algorithm ... 
    } 
public: 
    void DoSomething(T* pObj) 
    { 
        DoSomething(pObj, Int2Type<isPolymorphic>()); 
    } 
};
```
<details>
<summary>
It's an obsolete way of doing so. Nowadays, we use constexpr-if. (since C++ 17)
</summary>

```C++
class A{
    public:
    void foo(){
        cout << "foo" << endl;
    }
};
class B{
    public:
    void bar(){
        cout << "bar" << endl;
    }
};

template<bool isA, typename T>
void func(T t){
    if constexpr(isA){
        t.foo();
    }else{
        t.bar();
    }
}

int main()
{
    A a;
    B b;
    func<false>(b);
    func<true>(a);

    return 0;
}
```
</details>

<details>
<summary>
Adding on top of that, we can use is_same_v
</summary>

*Left class A and B unchanged.*

```C++

template<typename T>
void func(T t){
    if constexpr(is_same_v<T, A>){
        t.foo();
    }else{
        t.bar();
    }
}

int main()
{
    A a;
    B b;
    func(b);
    func(a);

    return 0;
}
```
</details>

#### Mapping Types to Types

This helps to achieve pseudo partial specification on templated meta functions.

We want to achieve this

```C++
template <class T, class U> 
T* Create(const U& arg) 
{ 
 return new T(arg); 
} 
```
However, the function doesn't allow us to specify what T is. Thus, we have to go overloading. 

An easy way to achieve so is specifying a dummy object in the argument

```C++
template <class T, class U> 
T* Create(const U& arg, **T** /* dummy */) 
{ 
 return new T(arg); 
} 
template <class U> 
Widget* Create(const U& arg, **Widget** /* dummy */) 
{ 
 return new Widget(arg, -1); 
} 
```
However, it has overhead of constructing gan arbitrarily complex object. We can have a light weight subtype, generated by the type we want, to do overloading. 

```C++
template <typename T> 
struct Type2Type 
{ 
 typedef T OriginalType; 
}; 
template <class T, class U> 
T* Create(const U& arg, Type2Type<T>) 
{ 
 return new T(arg); 
} 

template <class U> 
Widget* Create(const U& arg, Type2Type<Widget>) 
{ 
 return new Widget(arg, -1); 
} 
// Use Create() 
String* pStr = Create("Hello", Type2Type<String>()); 
Widget* pW = Create(100, Type2Type<Widget>());
```

#### Define variable type based on bool flag
If we want to define our value type to be T or T*, based on an isPolymorphic flag, we can do something like below. 

```C++
template <typename T, bool isPolymorphic> 
struct NiftyContainerValueTraits 
{ 
 typedef T* ValueType; 
}; 
template <typename T> 
struct NiftyContainerValueTraits<T, false> 
{ 
 typedef T ValueType; 
}; 
template <typename T, bool isPolymorphic> 
class NiftyContainer 
{ 
 ... 
 typedef NiftyContainerValueTraits<T, isPolymorphic> 
 Traits; 
 typedef typename Traits::ValueType ValueType; 
}; 
```
The problem with this is that for each new type, we must define it's trait class. 
* However, it's super common. I saw this in HRT's  workspace.

A slightly better solution proposed by the book

```C++
template <bool flag, typename T, typename U> 
struct Select 
{ 
 typedef T Result; 
}; 
template <typename T, typename U> 
struct Select<false, T, U> 
{ 
 typedef U Result; 
}; 

template <typename T, bool isPolymorphic> 
class NiftyContainer 
{ 
 ... 
 typedef typename Select<isPolymorphic, T*, T>::Result 
 ValueType; 
 ... 
}; 
```

Here, it looks like an if-else.

#### Inheritance clarification and type conclude*; DECLTYPE
In chapter 2.7, the book describes a way to distinguish if a class has an inheritance relationship with another class.

I found it's too old. I didn't finish this chapter lol.

Nowadays, we could use `is_same_v` and `decltype` to achieve the same effect at the compile time. (For determing if an object is a certain type.)

* The next section talks about how to determine in run time. (Useful for developing. Not meant to use in production)


```C++
class A{};
class B: public A{};
class C: public A{};
int main()
{
    A* a1 = new A();
    B b;
    A a2;

    static_assert(is_same_v<decltype(b), B>);
    static_assert(is_same_v<remove_pointer_t<decltype(a1)>, A>);
    // decltype(a1) generates A*
    // static_assert(is_same_v<decltype(a1), A>);
    // error: static assertion failed due to requirement 'is_same_v<A *, A>'
    // we remove the pointer from it
    static_assert(is_same_v<remove_reference_t<decltype(*a1)>, A>);
    // The dereference operator * returns a reference, which you cannot allocate using new
    // static_assert(is_same_v<decltype(*a1), A>);
    // error: static assertion failed due to requirement 'is_same_v<A &, A>'
    // we remove the reference from it
    
    return 0;
}
```
**Note**, I haven't found a way to detect the actual type about polymorphism

#### type_info and typeid
investigate object types at runtime
The typeid operator returns a reference to a type_info object: 

```C++
void Fun(Base* pObj) 
{ 
 // Compare the two type_info objects corresponding 
 // to the type of *pObj and Derived 
 if (typeid(*pObj) == typeid(Derived)) 
 { 
 ... aha, pObj actually points to a Derived object ... 
 } 
 ... 
}
```
```C++
A* a1 = new B();
B b;
A a2;
// 0
cout << (typeid(*a1) == typeid(b)) << endl;
// 1
cout << (typeid(*a1) == typeid(a2)) << endl;
```
~~Sadly it still doesn't support polymorphism~~ That's not the way to think about it. Because a1 is an A pointer, it can only see members of A (or any classes that A inherited). It would not know what's in B. (Except through virtual functions also defined in A.) 

### Type Trait
We could get the type of the objects with the help from template pattern matching. 

<details>

<summary>
Prior to C++ 17 / the reasoning
</summary>

``` C++
#include <iostream>
#include <type_traits>
using namespace std;

struct NullType{};

template <typename T> 
class TypeTraits 
{ 
private: 
    template <class U> struct PointerTraits 
    { 
        // originally, it's `result = false` here
        enum { value = 0 }; 
        typedef NullType PointeeType; 
    }; 
    // <class U> is required here because otherwise we don't know what U is.
    // the usage is like: we will accept a whatever type, and we name it U. 
    // put it in <class U> to declare it and make it available
    template <class U> struct PointerTraits<U*>
    { 
        enum { value = 1 }; 
        typedef U PointeeType; 
    }; 
    // if we talk about some thing we already know, we can keep it empty
    template <> struct PointerTraits<string*>
    { 
        enum { value = 2 }; 
        typedef string* PointeeType; 
    }; 
public: 
    enum { value = PointerTraits<T>::value }; 
    typedef typename PointerTraits<T>::PointeeType PointeeType; 
 
}; 

int main()
{
    cout << typeid(TypeTraits<int*>::PointeeType).name() << endl;
    cout << typeid(TypeTraits<int>::PointeeType).name() << endl;
    cout << TypeTraits<int*>::value << endl;
    cout << TypeTraits<int>::value << endl;
    // i   
    // 8NullType
    // 1
    // 0

    cout << typeid(TypeTraits<string*>::PointeeType).name() << endl;
    cout << typeid(TypeTraits<string>::PointeeType).name() << endl;
    cout << TypeTraits<string*>::value << endl;
    cout << TypeTraits<string>::value << endl;
    
    // PNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
    // 8NullType
    // 2
    // 0
    return 0;

/*
    const bool iterIsPtr = TypeTraits<vector<int>::iterator>::isPointer; 
    cout << "vector<int>::iterator is " << 
    (iterIsPtr ? "fast" : "smart") << '\n'; 
    // vector<int>::iterator is smart
    the iterator is not a pointer
*/
}
```
</details>

In C++ 17, we use the type_trait library.
```C++
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;

int main()
{
   cout << is_pointer_v<string*> << endl;
   cout << is_pointer_v<vector<int>::iterator> << endl;
}
```

### Type List
Here's the type of code I wrote in HRT

```C++
#include <iostream>

using namespace std;

struct A{
    void work(){
        cout << "Here's A" << endl;
    }
};

struct B{
    void work(){
        cout << "Here's B" << endl;
    }
};

template <typename... T>
struct TypeList{};

template <typename Module, typename... RemainingModules>
struct ModuleHolder: ModuleHolder<RemainingModules...>{
    using Base = ModuleHolder<RemainingModules...>;
    void foo(){
        Base::foo();
        m.work();
    }
    Module m;
};

template <typename Module>
struct ModuleHolder<Module>{
    void foo(){
        m.work();
    }
    Module m;
};

template <typename T>
struct MultiTypes{};

template <typename... submodules>
struct MultiTypes<TypeList<submodules...>>: ModuleHolder<submodules...>{
    using Base = ModuleHolder<submodules...>;
    using Base::Base;
};

int main() {
    MultiTypes<TypeList<A, B>> m;
    m.foo();
    // Here's B
    // Here's A
    return 0;
}
```
In explanation: 
* struct A and B are two different classes implementing work()
* TypeList is only a holder, with variadic variables `<typename ... T>`
* ModuleHolder is where we disassembly the variables, strip one variable at a time, through this line
  * `template <typename Module, typename... RemainingModules>`
  * It defines a foo() function, where it inherits from it self, and calls all subsequent types, that defined in the TypeList
  * It all goes eventually to ModuleHolder<Module>, which is a better match for a template with only one argument
* MultiType actually extract everything out of TypeList by specifying `TypeList` in the line here
  *  `template <typename... submodules>`
    `struct MultiTypes<TypeList<submodules...>>: ModuleHolder<submodules...>`
  * It forces `submodules` be the actual modules we want.
  * It also inherits from `ModuleHolder`, which makes us can simply call m.foo(). foo() is defined in ModuleHolder
  * In MultiType, **using Base::Base** allow us to call `.foo()` from this class
* Why do we have to define a **TypeList**?
  * We have to have this line, first of all.
  * `template <typename T> struct MultiTypes{};`
  * We want to know what are the submodules we are interested in
  * The main purpose of TypeList is to extract the modules and place it in our moduleHolder
  * If we write this line for MultiTypes: `struct MultiTypes<submodules...>: ModuleHolder<submodules...>`
  * The compiler cannot distinguish between it and the primary type when we only have one thing inside. This is because T and submodules are all non-existing types.

To have a length of the typelist, modify struct ModuleHolder like below

```C++
template <typename Module, typename... RemainingModules>
struct ModuleHolder: ModuleHolder<RemainingModules...>{
    using Base = ModuleHolder<RemainingModules...>;
    void foo(){
        Base::foo();
        m.work();
    }
    **enum {len = 1 + Base::len};**
    Module m;
};

template <typename Module>
struct ModuleHolder<Module>{
    void foo(){
        m.work();
    }
    **enum {len = 1};**
    Module m;
};
...
MultiTypes<TypeList<A, B, A, A, B, B>> m;
cout << m.len << endl;
```

----
Okay, the content in between is not complete. The book proposed a long typelist, which I don't think is useful.
The typelist supports adding/removing/searching operations. If needed, look for it in the future.
It basically works as a primitive data structures
https://codereview.stackexchange.com/questions/269320/c17-typelist-manipulation
A good article discussing typelist

#### Call a member function based on given variable?

Given a struct `WidgetFactory`, who has three different functions
``` C++
class WidgetFactory 
{ 
public: 
 virtual Window* CreateWindow() = 0; 
 virtual Button* CreateButton() = 0; 
 virtual ScrollBar* CreateScrollBar() = 0; 
};
```

We want to provide something like this, calling a particular function based on type.
```C++
template <class T> 
T* MakeRedWidget(WidgetFactory& factory) 
{ 
 T* pW = factory.CreateT(); // huh???
 pW->SetColor(RED); 
 return pW; 
} 

```
**But**, this is not allowed.

We can do this by falling factory.create<T>()...

----


### Template across different files
Normally, we define a template in the header file. However, we could do something as below to constraint the specialization of templates

```C++
// sq.h
template <typename T>
T sq(const T& x);


// sq.cpp
#include "sq.h"
template <typename T>
T sq(const T& x)
{
    return x*x; 
}
template double sq<double>(const double&);
```
This hides all definition inside the .cpp file. (I have no fucking idea why)
Except the last line. 
The last line expose a variation of sq, with T = double. 
Then in main, we have code as below

```C++
#include <iostream>
using namespace std;
#include "sq.h" // note: function body not visible
int main() {
   double x = sq(3.14); // with sq.h compiles and link for double
   // int y = sq(3);       // error, no instantiation for int sq<int>(int const&)
   cout << x << endl;
   // cout << y << endl;
}
```
calling `sq(3.14)` works because we exposed the version for double. But `sq(3)` doesn't because we never exposed int.
We **force the instantiation** of a template entity **in a translation unit** without 
ever using it.

### key words

#### typename

a dependent name means its meaning depends on T, which is an unknown variable
Example:
```C++
template <typename T>
void SomeFunc()
{
 MyClass<T>::Y * Q; 
};
```
The line could have two meanings:
* the declaration of local pointer-to-double named Q;
* or the product of the constant 314, times the global variable Q

Thus, for `MyClass<T>::Y` to refer a type, it must be introduced with the typename keyword

```C++
template <typename X>
class AnotherClass
{
 MyClass<X>::Type t1_; // error: 'Type' is a dependent name
 typename MyClass<X>::Type t2_; // ok
 MyClass<double>::Type t3_; // ok: 'Type' is independent of X
};
```

typename may introduce a new dependent type, that depends on it
```C++
template <typename T, typename T::type N>
struct SomeClass
{
};
struct S1
{
 typedef int type;
};
int main()
{
    // SomeClass<S1, 3.5> x;
    // invalid, S1::type is an int, not a double
    SomeClass<S1, 3> x;
}
```

### Angle Brackets
Prior to C++17, even if all parameters have a default value, you cannot entirely omit the angle brackets.
``` C++
template <typename T = double>
class sum {};
sum<> S1; // ok, using double
sum S2; // error
// error: use of class template 'sum' requires template arguments
```
It's never been a problem for templated function though. The code below works just fine
``` C++
template <typename T = double>
void foo() {};
foo<>(); 
foo(); 
```
**I don't think it's ever a problem with C++17**

### Universal constructor??

A **template copy constructor** and an assignment **are not called** when dealing with two objects of the **very same kind**. In another word, in the scenario where a normal copy constructor should be called.

```C++
template <typename T>
class something
{
public:
    something(){}
    // not called when S == T
    template <typename S>
    something(const something<S>& that)
    {
        cout << "template copy" << endl;
    }
 
    // An incorrect way
    //  something(const something<T>& that)
    //  {
    //     cout << "normal copy" << endl;
    //  }

    something(const something& that)
    {
        cout << "normal copy" << endl;
    }
    // not called when S == T
    template <typename S>
    something& operator=(const something<S>& that)
    {
        cout << "template assignment" << endl;
        return *this;
    }

    something& operator=(const something& that)
    {
        cout << "normal assignment" << endl;
        return *this;
    }
};
int main()
{
    something<int> s0;
    something<double> s1, s2;
    // template assignment
    s0 = s1; // calls user defined operator=
    // normal assignment
    s1 = s2; // calls the default assignment
}
```
A quick reference: https://stackoverflow.com/questions/19167201/copy-constructor-of-template-class
* even though A is a template, when you refer to it inside the class as A (such as in the function signatures) it is treated as the full type A<T>.

#### So, how do we write a constructor with universal reference and perfect forwarding?

```C++
template <typename D, typename S>
struct MyClass {
    template <typename DD, typename SS>
    constexpr MyClass(DD &&d, SS &&s): d_(std::forward<DD>(d)), s_(std::forward<SS>(s)) {}
private:
    D d_;
    S s_;
};

int main()
{
    auto c1 = MyClass<int, string>(1, std::string("Hello"));    // rvalues
    auto idx = 2;
    auto msg = std::string("World");
    auto c2 = MyClass<int, string>(idx, msg);    // lvalues
}
```
https://stackoverflow.com/questions/64262171/how-to-write-a-constructor-for-a-template-class-using-universal-reference-argume
Here, have to specify variable types in main function to call it. This is to enable type deduction. As Scott Meyer introduces, to make `&&` universal reference, it has to have type deduction also enabled.

If we remove the type there, as below
```C++
template <typename D, typename S>
struct MyClass {
    constexpr MyClass(D &&d, S &&s): d_(move(d)), s_(move(s)) {}
private:
    D d_;
    S s_;
};
```
This will error for the line 
`auto c2 = MyClass<int, string>(idx, msg);`
Because the constructor is not a universal constructor. Instead, it's a rvalue constructor only. While c2 wants to call a constructor with lvalues.

### Universal reference
A long article
https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
../../universal_reference.md 