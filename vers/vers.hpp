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
#include <memory>

namespace dde{

class vers{
    using void_p_t  = void*;
    using void_fp_t = void(*)(void*);
    using c_type_t  = const std::type_info;
    
    struct Storage{
        void_p_t  type;
        void_fp_t dest;
        c_type_t* type_info;
    } *stg;
    
public:
    ~vers();
    vers();
    
    template <typename Te>
    vers(const Te &t) : vers(){
        this->set<Te>(t);
    }
    
    vers(vers &&vs) noexcept : vers(){
        this->DestroyEvrything();
        
        this->stg = vs.stg;
        vs.stg    = nullptr;
    }
    
    template <typename Te>
    vers &operator= (const Te &t){
        return this->set<Te>(t);
    }
    
    vers &operator= (vers &&vs) noexcept{
        this->DestroyEvrything();
        
        this->stg = vs.stg;
        vs.stg    = nullptr;
        return *this;
    }
    
    template <typename Te>
    Te &get(){
        return *static_cast<Te*>(this->GetStg().type);
    }
    
    c_type_t &getTypeId(){
        return *this->GetStg().type_info;
    }
    
    bool empty() const{
        if (this->stg == nullptr)
            return true;
        return !(this->stg->type != nullptr);
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
    
    template <typename Te>
    vers &set(Te &&t){
        this->Destroy();
        
    }
    
private:
    Storage &GetStg(){
        return *this->stg;
    }
    
    template <typename Te>
    bool AllocateMemory_Init(const Te &t){
        if (this->empty()){
            this->GetStg().type = static_cast<Te*>(std::malloc(sizeof(Te)));
            std::construct_at<Te>(static_cast<Te*>(this->GetStg().type), t);
            this->GetStg().type_info = &typeid(t);
            
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
            
            if (this->GetStg().type != nullptr){
                std::free(this->GetStg().type);
                this->GetStg().type = nullptr;
            }
        }
    }
    
    void DestroyEvrything(){
        this->Destroy();
        if (this->stg != nullptr){
            std::free(this->stg);
            this->stg = nullptr;
        }
    }
};


};
#endif /* vers_hpp */
