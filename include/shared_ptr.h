#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_
/**
 * @file shared_ptr.h
 * @author LinuxYn (acmerzhangxipeng@163.com)
 * @brief 智能指针 shared_ptr 实现
 * @version 0.1
 * @date 2022-09-03
 * 参考实现：libstdc++6源 gcc-10-10.3.0/gcc-10.3.0/libstdc++-v3/include/bits
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <mutex>

class Shared_count {
    typedef int element_type;
public:
     constexpr Shared_count() noexcept
     : pi(0) {

     }

     explicit Shared_count(element_type _pi):pi(_pi) {

     }

     void reduce_count() {
        pi--;
     }

     void add_count() {
        pi++;
     }

    bool unique() const noexcept {
        return pi == 1;
    }

    void swap(Shared_count* p) noexcept {
        std::swap(pi, p->pi);
    }

    long use_count() const noexcept {
        return pi;
    }
private:
    element_type pi;
};

/**
 * @brief 智能指针 shared_ptr
 * 
 * @tparam T 
 */
template <typename T>
class Shared_ptr {
public:
    typedef Shared_count reference_count_type;
    typedef T element_type;

public:
    /**
     * @brief 默认构造函数
     * 添加 constexpr 的原因：
     * 添加 noexceppt 的原因：
     */
    constexpr Shared_ptr() noexcept
    :ptr(nullptr), ref_count(new reference_count_type(0)), m(new std::mutex) {
        std::cout<<"Shared_ptr()"<<std::endl;
    }

    /**
     * @brief Construct a new Shared_ptr object
     * 添加 explicit 的原因：
     * @param _ptr 
     */
    explicit Shared_ptr(T* _ptr)
    :ptr(_ptr),ref_count(new reference_count_type(1)),m(new std::mutex) {
        std::cout<<"Shared_ptr(T* _ptr)"<<std::endl;
    }
    
    //拷贝构造函数
    Shared_ptr(const Shared_ptr<T>& p) noexcept
    :ptr(p.ptr), ref_count(p.num), m(p.m) {
        ref_count->add_count();
    }

    //赋值构造函数
    Shared_ptr& operator=(Shared_ptr& p) noexcept {
        std::cout<<"SharedPtr operator="<<std::endl;
        if(p.ptr != ptr) {
            ref_count->reduce_count();
            ptr = p.ptr;
            ref_count = p.ref_count;
            m = p.m;
            ref_count->add_count();
        }
        return *this;
    }

    //移动构造函数
    Shared_ptr(Shared_ptr&& p) noexcept
    : ptr(p.ptr), ref_count(p.ref_count),m(p.m) {
        p.ptr = nullptr;
        p.ref_count = nullptr;
        p.m = nullptr;
    }

    //移动赋值函数
    Shared_ptr& operator=(Shared_ptr&& p) noexcept {
        if(p.ptr != ptr) {
            ref_count->reduce_count();
            ptr = p.ptr;
            ref_count = p.ref_count;
            m = p.m;
            ref_count->add_count();
        }
        return *this;
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    /**
     * @brief 获取存储的指针 ok
     * 
     * @return T*
     */
    T* get() const noexcept {
        return ptr;
    }

    //返回引用计数 ok
    long use_count() const noexcept {
        return ref_count->use_count();
    }

    //查询引用计数是否唯一 ok
    bool unique() const noexcept {
        return ref_count->unique();
    }

    // 如果存储的指针不为空，返回 true  ok
    explicit operator bool() const { 
        return ptr == nullptr ? false : true;
    }

    // 交换数据 ok
    void swap(Shared_ptr& other) noexcept {
	    std::swap(ptr, other.ptr);
        ref_count->swap(other.ref_count);
        std::swap(m, other.m);
    }
    
    //析构函数
    ~Shared_ptr() {
        std::cout<<"~Shared_ptr start"<<std::endl;
        if(ref_count->use_count()) {
            ref_count->reduce_count();
            if(ref_count->use_count() == 0 && ptr != nullptr) {
                delete ptr;
                delete ref_count;
                delete m;
                ptr = nullptr;
                ref_count = nullptr;
                m = nullptr;
            }
        }
        
        std::cout<<"~Shared_ptr end"<<std::endl;
    }

private:
    element_type* ptr;
    reference_count_type* ref_count;
    std::mutex* m;
};

#endif