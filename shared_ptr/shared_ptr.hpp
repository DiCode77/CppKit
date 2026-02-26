//
//  shared_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef shared_ptr_hpp
#define shared_ptr_hpp

namespace dde{

template <typename Te>
class shared_ptr{
    using ulong_t = unsigned long;
    
    ulong_t *strong;
    ulong_t *weak;
    Te      *data;
public:
    shared_ptr() : strong(nullptr), weak(nullptr), data(nullptr){}
    shared_ptr(Te *t) : shared_ptr(){
        if (t != nullptr){
            this->strong = new ulong_t(1);
            this->weak   = new ulong_t(0);
            this->data   = t;
        }
    }
    
    shared_ptr(const shared_ptr &sh_ptr) : shared_ptr(){
        if (!sh_ptr.empty()){
            this->strong = sh_ptr.strong;
            this->weak   = sh_ptr.weak;
            this->data   = sh_ptr.data;
            
            *this->strong += 1;
        }
    }
    
    shared_ptr(shared_ptr &&sh_ptr) : shared_ptr(){
        if (!sh_ptr.empty()){
            this->strong = sh_ptr.strong;
            this->weak   = sh_ptr.weak;
            this->data   = sh_ptr.data;
            
            sh_ptr.strong = nullptr;
            sh_ptr.weak   = nullptr;
            sh_ptr.data   = nullptr;
        }
    }
    
    ~shared_ptr(){
        if (!this->empty()){
            *this->strong -= 1;
            
            if (!*this->strong){
                delete this->strong;
                delete this->weak;
                delete this->data;
            }
        }
    }
    
    bool empty() const{
        return (this->data == nullptr) ? true : false;
    }
};

}

#endif /* shared_ptr_hpp */
