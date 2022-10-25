#include <iostream>
using namespace std;
// #include "sq_error.h" // note: function body not visible
#include "sq.h" // note: function body not visible
int main() {
//    double x = sq(3.14); // with sq_error.h compiles but does not link
   double x = sq(3.14); // with sq.h compiles and link for double
   int y = sq(3);       // error, no instantiation for int sq<int>(int const&)
   cout << x << endl;
   cout << y << endl;
}