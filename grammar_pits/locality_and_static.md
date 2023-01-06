Members and functions defined in an unnamed namespace are accessible in the same class. 
Unnamed namespace make an identifier *translation unit local*. 
Static global variables are also translation unit local
Using an unnamed namespace is equivalent of follow
```C++
namespace __unique_compiler_generated_identifer0x42 {
    ...
}
using namespace __unique_compiler_generated_identifer0x42;
```