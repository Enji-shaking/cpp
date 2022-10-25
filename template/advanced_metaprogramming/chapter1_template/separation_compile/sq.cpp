#include "sq.h"

template <typename T>
T sq(const T& x)
{
    return x*x; 
}
// with this, not okay

// this export the instantiation for doubles
template double sq<double>(const double&);
