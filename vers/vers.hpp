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
    enum class TYPE{
        NONE,
        BOOL,
        CHAR,
        INT,
        LONG,
        DOUBLE
    };
    
    struct data{
        void *s;
        TYPE  t;
        void (*dets)(void*) = nullptr;
    } t_info;
    
public:
    vers() : t_info(nullptr, TYPE::NONE){}
    
    template <typename Te>
    vers(Te t) : vers(){
        this->t_info.s = (Te*)std::malloc(sizeof(Te));
        new(this->t_info.s) Te(t);
        
        this->t_info.dets = [](void *p){
            Te *cs = static_cast<Te*>(p);
            cs->~Te();
        };
    }
    
    ~vers(){
        this->Destroy(this->t_info.s);
    }
    
    template <typename Te>
    vers &operator= (Te t){
        if (this->t_info.dets != nullptr){
            this->Destroy(this->t_info.s);
        }
        
        this->t_info.s = (Te*)std::malloc(sizeof(Te));
        new(this->t_info.s) Te(std::move(t));
        
        this->t_info.dets = [](void *p){
            Te *cs = reinterpret_cast<Te*>(p);
            cs->~Te();
        };
        
        return *this;
    }
    
    template <typename Te>
    Te &get(){
        return *static_cast<Te*>(this->t_info.s);
    }
    
private:
    void Destroy(void *p){
        if (this->t_info.dets != nullptr){
            this->t_info.dets(p);
        }
    }
};


};
#endif /* vers_hpp */
