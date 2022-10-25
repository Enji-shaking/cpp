#include <iostream>

using namespace std;


template <class To, class From>
To safe_cast_compile_time(From from)
{
    // using static_assert to check the size of the types at compile time
    static_assert(sizeof(From) >= sizeof(To), "size less");
    return static_cast<To>(from);
}

template <class To, class From>
To safe_cast_run_time(From from)
{
    // using static_assert to check the size of the types at compile time
    assert(sizeof(From) >= sizeof(To));
    return static_cast<To>(from);
}

int main(){
    int a = 1;
    double b = safe_cast_run_time<double>(a);
    cout << b << endl;
    return 0;
}