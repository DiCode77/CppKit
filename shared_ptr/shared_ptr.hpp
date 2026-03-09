//
//  shared_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef shared_ptr_hpp
#define shared_ptr_hpp

namespace dde{

template<typename T>
class weak_ptr;

template <typename Te>
class shared_ptr{
    using ulong_t = unsigned long;
    
    struct CONTROL_BK{
        ulong_t strong;
        ulong_t weak;
        Te      *data;
        
        CONTROL_BK(Te *t, ulong_t sg = 1, ulong_t wk = 0) : strong(sg), weak(wk), data(t){}
        
    } *con_bk;
    
public:
    shared_ptr() : con_bk(nullptr) {}
    explicit shared_ptr(Te *t) : shared_ptr(){
        if (t != nullptr){
            this->con_bk = new CONTROL_BK(t);
        }
    }
    
    explicit shared_ptr(CONTROL_BK *c_bk) : shared_ptr(){
        if (c_bk != nullptr){
            this->con_bk = c_bk;
            this->con_bk->strong++;
        }
    }
    
    shared_ptr(const shared_ptr &sh_ptr) : shared_ptr(){
        if (!sh_ptr.empty()){
            this->con_bk = sh_ptr.con_bk;
            this->con_bk->strong++;
        }
    }
    
    shared_ptr(shared_ptr &&sh_ptr) noexcept : shared_ptr(){
        if (!sh_ptr.empty()){
            this->con_bk = sh_ptr.con_bk;
            sh_ptr.con_bk = nullptr;
        }
    }
    
    ~shared_ptr(){
        this->Release();
    }
    
    bool empty() const{
        if (this->con_bk == nullptr)
            return true;
        return (this->con_bk->data == nullptr) ? true : false;
    }
    
    void reset(){
        this->reset(nullptr);
    }
    
    void reset(Te *t){
        if (t != nullptr){
            this->Release();
            this->con_bk = new CONTROL_BK(t);
        }
        else{
            this->Release();
            
            if (!this->empty()){
                this->con_bk = nullptr;
            }
        }
    }
    
    void swap(shared_ptr &sh_ptr){
        CONTROL_BK *tmp = this->con_bk;
        
        this->con_bk  = sh_ptr.con_bk;
        sh_ptr.con_bk = tmp;
    }
    
    Te *get() const{
        return !this->empty() ? this->con_bk->data : nullptr;
    }
    
    ulong_t use_count() const{
        return !this->empty() ? this->con_bk->strong : 0;
    }
    
    explicit operator bool() const noexcept{
        return !this->empty();
    }
    
    Te &operator*() const noexcept{
        return *this->con_bk->data;
    }
    
    Te *operator->() const noexcept{
        return this->con_bk->data;
    }
    
    shared_ptr& operator= (const shared_ptr &sh_ptr){
        if (this != &sh_ptr){
            if (!this->empty()){
                this->Release();
            }
            
            this->con_bk = sh_ptr.con_bk;
            this->con_bk->strong += 1;
        }
        return *this;
    }
    
    shared_ptr& operator= (shared_ptr &&sh_ptr){
        if (this != &sh_ptr){
            if (!this->empty()){
                this->Release();
            }
            
            this->con_bk = sh_ptr.con_bk;
            sh_ptr.con_bk = nullptr;
        }
        return *this;
    }
    
    friend class dde::weak_ptr<Te>;
private:
    void Release(){
        if (!this->empty()){
            if (this->con_bk->strong > 0)
                this->con_bk->strong -= 1;
            
            if (!this->con_bk->strong){
                delete this->con_bk->data;
                this->con_bk->data = nullptr;
            }
            
            if (!this->con_bk->strong && !this->con_bk->weak){
                delete this->con_bk;
                this->con_bk = nullptr;
            }
        }
    }
};

}

#endif /* shared_ptr_hpp */
