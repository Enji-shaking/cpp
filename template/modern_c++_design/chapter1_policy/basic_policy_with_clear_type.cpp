#include <iostream>
using namespace std;

template <typename T>
struct OpNewCreator 
{ 
    static T* Create() { 
        return new T; 
    } 
};

struct Widget{
    void print(){
        cout << "print from widget template template" << endl;
    }
};

// template template parameter
// this Created is just a place holder
// this (likely) utilizes the type 
template <template <typename bluhbluh> typename CreationPolicy>
struct WidgetManager : public  CreationPolicy<Widget>
{ 
 void DoSomething() 
 {
    Widget* ptr = CreationPolicy<Widget>::Create();
    ptr->print(); 
 } 
};

template <typename T, typename U>
struct testPrinter 
{ 
    void print(){
        cout << "T, U" << endl;
    }
};

template <typename T>
struct testPrinter<T, char>
{ 
    void print(){
        cout << "T, char" << endl;
    }
};

template <>
struct testPrinter<int, int>
{ 
    void print(){
        cout << "int int" << endl;
    }
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer1 : public PrinterPolicy<int, char>
{ 
    typedef PrinterPolicy<int, char> type;
    void DoSomething() 
    {
        type::print();
    } 
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer2 : public PrinterPolicy<int, int>
{ 
    typedef PrinterPolicy<int, int> type;
    void DoSomething() 
    {
        type::print();
    } 
};

template <template <typename, typename> typename PrinterPolicy>
struct printerContainer3 : public PrinterPolicy<string, string>
{ 
    typedef PrinterPolicy<string, string> type;
    void DoSomething() 
    {
        type::print();
    } 
};

int main(){
    WidgetManager<OpNewCreator> op;
    op.DoSomething();
    printerContainer1<testPrinter> t1;
    printerContainer2<testPrinter> t2;
    printerContainer3<testPrinter> t3;
    t1.DoSomething();
    t2.DoSomething();
    t3.DoSomething();
}