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
    ~vers(){
        this->Destroy(this->GetStg().type);
    }
    
    vers() : stg(nullptr, nullptr){}
    
    template <typename Te>
    vers(const Te t) : vers(){
        this->set<Te>(t);
    }
    
    template <typename Te>
    vers &operator= (const Te t){
        return this->set<Te>(t);
    }
    
    template <typename Te>
    Te &get(){
        return *static_cast<Te*>(this->GetStg().type);
    }
    
    bool empty() const{
        return !(this->stg.type != nullptr && this->stg.dest != nullptr);
    }
    
    template <typename Te>
    vers &set(Te t){
        this->Destroy(this->GetStg().type);
        if (this->AllocateMemory_Init<Te>(t)){
            this->ImplCaptureType<Te>();
        }
        
        return *this;
    }
    
private:
    Storage &GetStg(){
        return this->stg;
    }
    
    template <typename Te>
    bool AllocateMemory_Init(Te t){
        if (this->empty()){
            this->GetStg().type = static_cast<Te*>(std::malloc(sizeof(Te)));
            new(this->GetStg().type) Te(t);
            
            return true;
        }
        return false;
    }
    
    template <typename Te>
    void ImplCaptureType(){
        this->GetStg().dest = [](void_p_t p_te){
            static_cast<Te*>(p_te)->~Te();
        };
    }
    
    void Destroy(void_p_t p_te){
        if (!this->empty() && p_te != nullptr){
            this->GetStg().dest(p_te);
            this->GetStg().dest = nullptr;
            std::free(this->GetStg().type);
            this->GetStg().type = nullptr;
        }
    }
};


};
#endif /* vers_hpp */
