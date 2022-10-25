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
        cout << "print from widget" << endl;
    }
};

template <typename CreationPolicy> 
struct WidgetManager : public CreationPolicy
{ 
 void DoSomething() 
 {
    Widget* ptr = CreationPolicy::Create();
    ptr->print(); 
 } 
};

int main(){
    WidgetManager<OpNewCreator<Widget>> op;
    op.DoSomething();
}