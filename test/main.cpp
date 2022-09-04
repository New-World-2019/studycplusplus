
#include "shared_ptr.h"

/**
 * @brief 测试 shared_ptr 智能指针
 * 
 */
void test_Shared_ptr() {
    std::cout<<"function testSharedPtr"<<std::endl;
    // 默认构造函数测试
    Shared_ptr<int>p;
    Shared_ptr<int>p1(new int(1));
    std::cout<<*p1<<std::endl;
}

int main() {
    std::cout<<"Hello World"<<std::endl;
    test_Shared_ptr();
}
