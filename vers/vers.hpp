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
    } t_info;
    
public:
    vers() : t_info(nullptr, TYPE::NONE){}
    
    template <typename Te>
    vers(Te t) : vers(){
        this->t_info.s = (Te*)std::malloc(sizeof(Te));
        new(this->t_info.s) Te(t);
    }
    
    ~vers(){
        this->Destroy(this->t_info.s);
    }
    
    template <typename Te>
    vers &operator= (Te t){
        if (this->t_info.s != nullptr){
            this->Destroy<Te*>(this->t_info.s);
        }
        
        this->t_info.s = (Te*)std::malloc(sizeof(Te));
        new(this->t_info.s) Te(t);
        return *this;
    }
    
    
    template <typename Te>
    Te &get(){
        return *static_cast<Te*>(this->t_info.s);
    }
    
private:
    
    template <typename Te = void*>
    void Destroy(void *des){
        if (des != nullptr){
            Te *t = (Te*)des;
            t->~Te();
            std::free(des);
            
        }
    }
};


};
#endif /* vers_hpp */
