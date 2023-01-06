template <class T> class Widget 
{ 
 void Fun() { 
    // .. generic implementation ... 
} 
    static int a = 4;
}; 
// OK: specialization of a member function of Widget 
template <> void Widget<char>::Fun() 
{ 
//  ... specialized implementation ... 
} 
int Widget<char>::a = 5;

template <class T, class U> class Gadget 
{ 
 void Fun() { .. generic implementation ... } 
}; 
// Error! Cannot partially specialize a member class of Gadget 
template <class U> void Gadget<char, U>::Fun() 
{ 
//  ... specialized implementation ... 
} 

template <> void Gadget<char, int>::Fun() 
{ 
//  ... specialized implementation ... 
} 

template <typename T, typename U>
void dummy(){
    cout << "dummy" << endl;
}

template <typename U>
void dummy<int>(){
    cout << "dummy int" << endl;
}

template <>
void dummy<int, char>(){
    cout << "dummy int char" << endl;
}