#include <iostream>
using namespace std;
template <typename X>
struct outer
{
    template <typename T>
    static T inner(T x){
        return x+1; 
    }
};
typedef double (*FUNC_T)(double);

template <typename X>
void do_it()
{
// if they are members of class templates and their context depends on a yet unspecified parameter, they require an extra template keyword before their name

//    FUNC_T f1 = outer<X>::inner<double>; // error!
    FUNC_T f2 = outer<X>::template inner<double>; // correct
    // cout << f1(3) << endl;
    cout << f2(3) << endl;

}

int main(){
    FUNC_T f1 = outer<double>::inner<double>; // okay, as double is determined
    // FUNC_T f2 = outer<double>::template inner<double>; // error, redundant "template"
    cout << f1(3) << endl;
    // cout << f2(3) << endl;
    do_it<string>();
}