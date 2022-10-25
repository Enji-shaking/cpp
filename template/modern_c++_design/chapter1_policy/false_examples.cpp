template <class T> class Widget 
{ 
 void Fun() { 
    // .. generic implementation ... 
} 
}; 
// OK: specialization of a member function of Widget 
template <> void Widget<char>::Fun() 
{ 
//  ... specialized implementation ... 
} 
template <class T, class U> class Gadget 
{ 
 void Fun() { .. generic implementation ... } 
}; 
// Error! Cannot partially specialize a member class of Gadget 
template <class U> void Gadget<char, U>::Fun() 
{ 
//  ... specialized implementation ... 
} 