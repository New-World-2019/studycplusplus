#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_
/**
 * @file shared_ptr.h
 * @author LinuxYn (acmerzhangxipeng@163.com)
 * @brief 智能指针 shared_ptr 实现
 * @version 0.1
 * @date 2022-09-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <mutex>
using namespace std;

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* num;
    mutex* m;
private:
    void release() {
        cout<<"release()"<<endl;
        bool flag = false;
        m->lock();
        /**
         * @brief 注意：不能写成 *num--
         * 因为后缀--的优先级高于 *，所以需要添加括号 
         */
        (*num)--;
        if(*num == 0) {
            delete ptr;
            delete num;
            ptr = nullptr;
            num = nullptr;
            flag = true;
        }
        m->unlock();
        cout<<"flag"<<endl;
        if(flag) {
            delete m;
            m = nullptr;
        }
    }
    void addRef() {
        m->lock();
        *num++;
        m->unlock();
    }
public:
    SharedPtr(T* _ptr = nullptr):ptr(_ptr),num(new int(1)),m(new mutex) {
        cout<<"SharedPtr"<<endl;
    }
    SharedPtr(const SharedPtr<T>& p):ptr(p.ptr),num(p.num),m(p.m) {
        cout<<"SharedPtr(const SharedPtr)"<<endl;
        addRef();
    }

    SharedPtr<T>& operator=(SharedPtr<T>& p) {
        cout<<"SharedPtr operator="<<endl;
        if(p.ptr != ptr) {
            release();
            ptr = p.ptr;
            num = p.num;
            m = p.m;
            addRef();
        }
        return *this;
    }
    T& operator*() {
        return *ptr;
    }
    T* operator->() {
        return ptr;
    }
    ~SharedPtr() {
        cout<<"~SharedPtr"<<endl;
        release();
        cout<<"~SharedPtr after"<<endl;
    }

};

#endif