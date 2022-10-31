// page 21
#include <iostream>
using namespace std;

// (1) function template
template <typename scalar_t>
inline scalar_t sq(const scalar_t& x) {
    cout << "function 1" << endl;
    cout << x*x << endl;
    return x;
}
// (2) overload
inline double sq(const double& x){
    cout << "function 2" << endl;
    cout << x*x << endl;
    return x;
}

// (3) specialization of 1
template <>
inline int sq(const int& x){
    cout << "function 3" << endl;
    cout << x*x << endl;
    return x;
}

int main(){
    sq(3.0);  // function 2
    sq(3);    // function 3
    sq(3.0f); // function 1

}