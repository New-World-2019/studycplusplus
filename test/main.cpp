
#include "shared_ptr.h"

/**
 * @brief 测试 shared_ptr 智能指针
 * 
 */
void testSharedPtr() {
    cout<<"function testSharedPtr"<<endl;
    SharedPtr<int>p = new int(1);
    cout<<*p<<endl;
}

int main() {
    cout<<"Hello World"<<endl;
    testSharedPtr();
}
