#include <iostream>
using namespace std;

// global function template: outside
template <typename scalar_t>
inline scalar_t gsq(const scalar_t& x) { 
    cout << "function template in global" << endl;
    cout << x*x << endl;
    return x;
 };

// specialization: outside
template <>
inline int gsq(const int& x) { 
    cout << "function specification in global" << endl;
    cout << x*x << endl;
    return x;
 }

class Mathematics{
    public:
    // template member function
    template <typename scalar_t>
    inline scalar_t sq(const scalar_t& x){
        return gsq(x);
    }
};

// class Mathematics
// {
// public:
//  template <typename scalar_t>
//  inline scalar_t sq(const scalar_t& x) { 
//     cout << "function template in class" << endl;
//     cout << x*x << endl;
//     return x;
//   }; // template member function
//  template <>
//  inline int sq(const int& x) { 
//     cout << "function specification in class" << endl;
//     cout << x*x << endl;
//     return x;
//   }; // illegal specialization!
// };

int main(){
    Mathematics m;
    m.sq(3);
    m.sq(3.0);

}