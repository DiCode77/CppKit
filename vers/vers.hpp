//
//  vers.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef vers_hpp
#define vers_hpp
#include <typeinfo>
#include <cstdlib>
#include <utility>

namespace dde{

class vers{
    using void_p_t  = void*;
    using void_fp_t = void(*)(void*);
    
    struct Storage{
        void_p_t  type;
        void_fp_t dest;
    } stg;
    
public:
    vers() : stg(nullptr, nullptr){}
    
    template <typename Te>
    vers(Te t) : vers(){
        this->stg.type = static_cast<Te*>(std::malloc(sizeof(Te)));
        new(this->stg.type) Te(t);
        
        this->stg.dest = [](void_p_t p_te){
            static_cast<Te*>(p_te)->~Te();
        };
    }
    
    ~vers(){
        this->Destroy(this->stg.type);
    }
    
    template <typename Te>
    vers &operator= (Te t){
        if (!this->empty()){
            this->Destroy(this->stg.type);
        }
        
        this->stg.type = static_cast<Te*>(std::malloc(sizeof(Te)));
        new(this->stg.type) Te(t);
        
        this->stg.dest = [](void_p_t p_te){
            static_cast<Te*>(p_te)->~Te();
        };
        
        return *this;
    }
    
    template <typename Te>
    Te &get(){
        return *static_cast<Te*>(this->stg.type);
    }
    
    bool empty() const{
        return !(this->stg.type != nullptr && this->stg.dest != nullptr);
    }
    
private:
    void Destroy(void_p_t p_te){
        if (!this->empty()){
            this->stg.dest(p_te);
            this->stg.dest = nullptr;
            std::free(this->stg.type);
            this->stg.type = nullptr;
        }
    }
};


};
#endif /* vers_hpp */
