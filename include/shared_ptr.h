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

template <typename T>
class Shared_ptr {
public:
    typedef long reference_count_type;
    typedef T element_type;
private:
    //减少引用计数
    void reduce_count() {
        std::cout<<"reduce_count"<<std::endl;
        m->lock();
        /**
         * @brief 注意：不能写成 *num--
         * 因为后缀--的优先级高于 *，所以需要添加括号 
         */
        (*num)--;
        m->unlock();
    }

    //增加引用计数
    void add_count() {
        m->lock();
        (*num)++;
        m->unlock();
    }
public:
    /**
     * @brief 默认构造函数
     * 添加 constexpr 的原因：
     * 添加 noexceppt 的原因：
     */
    constexpr Shared_ptr() noexcept
    :ptr(nullptr),num(new reference_count_type(0)),m(new std::mutex) {
        std::cout<<"Shared_ptr()"<<std::endl;
    }

    /**
     * @brief Construct a new Shared_ptr object
     * 添加 explicit 的原因：
     * @param _ptr 
     */
    explicit Shared_ptr(T* _ptr)
    :ptr(_ptr),num(new reference_count_type(1)),m(new std::mutex) {
        std::cout<<"Shared_ptr(T* _ptr)"<<std::endl;
    }
    
    //拷贝构造函数
    Shared_ptr(const Shared_ptr<T>& p) noexcept
    :ptr(p.ptr),num(p.num),m(p.m) {
        add_count();
    }

    //赋值构造函数
    Shared_ptr& operator=(Shared_ptr& p) noexcept {
        std::cout<<"SharedPtr operator="<<std::endl;
        if(p.ptr != ptr) {
            reduce_count();
            ptr = p.ptr;
            num = p.num;
            m = p.m;
            add_count();
        }
        return *this;
    }

    //移动构造函数
    Shared_ptr(Shared_ptr&& p) noexcept
    : ptr(p.ptr), num(p.num),m(p.m) {
        p.ptr = nullptr;
        p.num = nullptr;
        p.m = nullptr;
    }

    //移动赋值函数
    Shared_ptr& operator=(Shared_ptr&& p) noexcept {
        if(p.ptr != ptr) {
            reduce_count();
            ptr = p.ptr;
            num = p.num;
            m = p.m;
            add_count();
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
     * @brief 获取存储的指针
     * 
     * @return T*
     */
    T* get() const noexcept {
        return ptr;
    }

    //返回引用计数
    reference_count_type use_count() const noexcept {
        return *num;
    }

    // 如果存储的指针不为空，返回 true
    explicit operator bool() const { 
        return ptr == nullptr ? false : true;
    }

    // 交换数据
    void swap(Shared_ptr& other) noexcept {
	    std::swap(ptr, other.ptr);
	    std::swap(num, other.num);
        std::swap(m, other.m);
    }
    
    //析构函数
    ~Shared_ptr() {
        std::cout<<"~Shared_ptr start"<<std::endl;
        std::cout<<"*num 1= "<<*num<<std::endl;
        if(*num) {
            reduce_count();
            std::cout<<"*num 2= "<<*num<<std::endl;
            if(*num == 0 && ptr != nullptr) {
                delete ptr;
                delete num;
                delete m;
                ptr = nullptr;
                num = nullptr;
                m = nullptr;
            }
        }
        
        std::cout<<"~Shared_ptr end"<<std::endl;
    }

private:
    element_type* ptr;
    reference_count_type* num;
    std::mutex* m;
};

#endif