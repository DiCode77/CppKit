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
#include <stdexcept>

namespace dde{

class vers{
    using void_p_t   = void*;
    using void_fp_t  = void(*)(void*);
    using void_cfp_t = void *(*)(const void*);
    using c_type_t   = const std::type_info;
    
    struct Storage{
        void_p_t   data;
        void_fp_t  dest;
        c_type_t   *type_info;
        void_cfp_t data_save;
    } *stg;
    
public:
    ~vers();
    vers();
    
    template <typename Te>
    vers(const Te &t) : vers(){
        this->set<Te>(t);
    }
    
    vers(const vers &vs) : vers(){
        this->CopyStorage(this->stg, vs.stg);
    }
    
    vers(vers &&vs) noexcept : vers(){
        this->DestroyEvrything();
        this->MoveStorage(&this->stg, &vs.stg);
    }
    
    template <typename Te>
    vers &operator= (const Te &t){
        return this->set<Te>(t);
    }
    
    vers &operator= (const vers &vs){
        return this->CopyStorage(this->stg, vs.stg);
    }
    
    vers &operator= (vers &&vs) noexcept{
        this->DestroyEvrything();
        return this->MoveStorage(&this->stg, &vs.stg);
    }
    
    template <typename Te>
    Te &get(){
        if (this->empty())
            throw std::runtime_error("The 'dde::vers' type is empty!");
        return *static_cast<Te*>(this->GetStg().data);
    }
    
    c_type_t &getTypeId(){
        return *this->GetStg().type_info;
    }
    
    bool empty() const{
        if (this->stg == nullptr)
            return true;
        return !(this->stg->data != nullptr);
    }
    
    template <typename Te> requires (!std::is_same_v<std::decay_t<Te>, dde::vers>)
    vers &set(const Te &t){
        this->Destroy();
        
        using Real = std::decay_t<decltype(t)>;
        if (this->AllocateMemory_Init<Real>(t)){
            this->ImplCaptureType<Real>();
        }
        return *this;
    }
    
    vers &set(const dde::vers &vs){
        return this->CopyStorage(this->stg, vs.stg);
    }
    
    vers &set(dde::vers &&vs){
        this->DestroyEvrything();
        return this->MoveStorage(&this->stg, &vs.stg);
    }
    
    
private:
    Storage *CreateStorage(){
        void *data = std::malloc(sizeof(dde::vers::Storage));
        std::construct_at<dde::vers::Storage>(static_cast<dde::vers::Storage*>(data), nullptr, nullptr, nullptr, nullptr);
        return static_cast<dde::vers::Storage*>(data);
    }
    
    Storage &GetStg(){
        return *this->stg;
    }
    
    template <typename Te>
    bool AllocateMemory_Init(const Te &t){
        if (this->empty()){
            this->GetStg().data = std::malloc(sizeof(Te));
            std::construct_at<Te>(static_cast<Te*>(this->GetStg().data), t);
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
        
        this->GetStg().data_save = [](const void *o_type) -> void*{
            void *type = std::malloc(sizeof(Te));
            std::construct_at<Te>(static_cast<Te*>(type), *static_cast<const Te*>(o_type));
            return type;
        };
    }
    
    void Destroy(){
        if (!this->empty()){
            this->GetStg().dest(this->GetStg().data);
            this->GetStg().dest = nullptr;
            
            if (this->GetStg().data != nullptr){
                std::free(this->GetStg().data);
                
                this->GetStg().data      = nullptr;
                this->GetStg().data_save = nullptr;
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
    
    dde::vers &CopyStorage(Storage *tstg, const Storage *ustg){
        if (tstg != nullptr && ustg != nullptr){
            tstg->data      = ustg->data_save(ustg->data);
            tstg->dest      = ustg->dest;
            tstg->type_info = ustg->type_info;
            tstg->data_save = ustg->data_save;
        }
        return *this;
    }
    
    dde::vers &MoveStorage(Storage **tstg, Storage **ustg){
        // *tstg must be null; otherwise, the condition will not be met. This is important to prevent memory leaks.
        if (*tstg == nullptr && *ustg != nullptr){
            *tstg = *ustg;
            *ustg = nullptr;
        }
        
        if (*ustg == nullptr){
            *ustg = this->CreateStorage();
        }
        
        return *this;
    }
};


};
#endif /* vers_hpp */
