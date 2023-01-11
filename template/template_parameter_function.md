### 1.1.4. Function Types and Function Pointers
(advanced-metaprogramming)

A type can take in both a function type or a function pointer type.  

```C++
template <double F(int)>
struct A
{
};
template <double (*F)(int)>
struct B
{
};
// They are mostly equivalent:
double f(int)
{
 return 3.14;
}
 A<f> t1; // ok
 B<f> t2; // ok
 ```
However, the difference is that **a function type cannot be constructed**.

```C++
template <typename T>
struct X
{
    T member_;
    X(T value)
    : member_(value)
    {
    }
};

double f(int){return 3.14;}
int main()
{
    X<double (*)(int)> t2(f);
    // X<double (int)> t1(f); // error: data member instantiated with function type 'double (int)'
    X t3(f);
}

```

If we specify the type being a function type, it would not work.
But of course you can let compile deduce it itself, which will decay it to a function pointer, as t3.

It happens when pass the function around
```C++
template <typename T>
X<T> identify_by_val(T x)
{
 return X<T>(x);
}
template <typename T>
X<T> identify_by_ref(const T& x)
{
 return X<T>(x);
}
double f(int)
{
 return 3.14;
}
identify_by_val(f); // function decays to pointer-to-function:
 // template instantiated with T = double (*)(int)
identify_by_ref(f); // no decay:
 // template instantiated with T = double (int)
```
Taking parameter in value will result in decay, which is fine.
Taking parameter in reference will not trigger the decay, which will error.

#### Dependent type on function
As other types, where a dependent type needs a `typename` prefix, we need a `template` prefix for a templated type that depends on a yet unspecified parameter

``` C++
typedef double (*FUNC_T)(double);

template <typename X>
struct outer
{
    template <typename T>
    static T g(T x)
    {
        return x+1;
    }

    template <typename Y>
    struct inner{
        const static int S = 3;
    };
};
template <typename X>
void do_it()
{
    FUNC_T f1 = outer<X>::g<double>; // error!
    FUNC_T f2 = outer<X>::template g<double>; // correct
    // int i = outer<X>::inner<double>::S;
    int i1 = outer<X>:: template inner<double>::S;
    int i2 = outer<int>::inner<double>::S
    typename outer<X>:: template inner<double> obj1;
    typename outer<double>::inner<double> obj2;
    outer<double>::inner<double> obj3;
}

```

* If a template resides in another template, that the outer template's type is not fully known, we need to add `template` key word after `outer<x>` to tell the compiler what ever after this is a template.
* You don't need this keyword if the outer is defined
* You also need `typename` for obj1 for almost the same reason, that you need keyword `typename` if the type is undetermined