https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers

“&&” in a type declaration sometimes means rvalue reference, but sometimes it means either rvalue reference or lvalue reference. As such, some occurrences of “&&” in source code may actually have the meaning of “&”

Rvalue references may bind only to rvalues
lvalue references, in addition to being able to bind to lvalues, may bind to rvalues only under restricted circumstances
references declared with “&&” that may be either lvalue references or rvalue references may bind to anything

If a variable or parameter is declared to have type **T&&** for some **deduced type** T, that variable or parameter is a **universal reference**.

If the expression initializing a universal reference is an lvalue, the universal reference becomes an lvalue reference.
If the expression initializing the universal reference is an rvalue, the universal reference becomes an rvalue reference.

If you can take the address of an expression, the expression is an lvalue.
If the type of an expression is an lvalue reference (e.g., T& or const T&, etc.), that expression is an lvalue. 
Otherwise, the expression is an rvalue.  Conceptually (and typically also in fact), rvalues correspond to **temporary objects**, such as those returned from functions or created through implicit type conversions. Most literal values (e.g., 10 and 5.3) are also rvalues.

