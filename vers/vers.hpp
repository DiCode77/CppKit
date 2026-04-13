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
#include <type_traits>

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
        this->Destroy();
    }
    
    vers() : stg(nullptr, nullptr){}
    
    template <typename Te>
    vers(const Te &t) : vers(){
        this->set<Te>(t);
    }
    
    template <typename Te>
    vers &operator= (const Te &t){
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
    vers &set(const Te &t){
        this->Destroy();
        
        using Real = std::decay_t<decltype(t)>;
        if (this->AllocateMemory_Init<Real>(t)){
            this->ImplCaptureType<Real>();
        }
        return *this;
    }
    
private:
    Storage &GetStg(){
        return this->stg;
    }
    
    template <typename Te>
    bool AllocateMemory_Init(const Te &t){
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
    
    void Destroy(){
        if (!this->empty()){
            this->GetStg().dest(this->GetStg().type);
            this->GetStg().dest = nullptr;
            std::free(this->GetStg().type);
            this->GetStg().type = nullptr;
        }
    }
};


};
#endif /* vers_hpp */
