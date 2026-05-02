//
//  vector.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef dde_vector_hpp
#define dde_vector_hpp

#include <cstdlib>
#include <memory>
#include <initializer_list>

namespace dde {

template <typename VecTe>
class vector{
    static constexpr const char *VEC_VERSION = "0.0.0-2b";
    
    using data_p_t = VecTe*;
    using ulong_t  = unsigned long;
    
    struct Storage{
        data_p_t data;
        ulong_t  size;
        ulong_t  capacity;
    } *stg;
    
public:
    vector() : stg(this->CreateStorage(nullptr, 0, 0)){}
    explicit vector(const ulong_t &in_size_vec) : vector(){
        this->stg->data     = this->InitializeArray(in_size_vec, {}, true);
        this->stg->size     = in_size_vec;
        this->stg->capacity = in_size_vec;
    }
    
    explicit vector(const ulong_t &in_size_vec, const VecTe &t_val) : vector(){
        this->stg->data     = this->InitializeArray(in_size_vec, t_val, true);
        this->stg->size     = in_size_vec;
        this->stg->capacity = in_size_vec;
    }
    
    vector(std::initializer_list<VecTe> list) : vector(){}
    
    ~vector(){
        this->RemoveArray(this->stg->data, this->stg->capacity);
        std::free(this->stg);
    }
    
    bool empty() const{
        if (this->stg == nullptr)
            return true;
        return this->stg->data == nullptr;
    }
    
    ulong_t size() const{
        return this->empty() ? 0 : this->stg->size;
    }
    
    ulong_t capacity() const{
        return this->empty() ? 0 : this->stg->capacity;
    }
    
    VecTe &at(const ulong_t &pos){
        return *(this->stg->data + pos);
    }
    
    void resize(const ulong_t &resize){
        if (resize > this->capacity()){
            this->stg->capacity = resize;
            this->IncreaseDataVolume(&this->stg->data, this->size(), resize, this->capacity());
        }
        else{
            if constexpr (!std::is_trivially_destructible_v<VecTe>){
                for (ulong_t i = resize; i < this->size(); i++){
                    std::destroy_at<VecTe>(this->stg->data + i);
                }
            }
            std::memset(reinterpret_cast<void*>(this->stg->data +resize), 0, sizeof(VecTe) * (this->size() - resize));
        }
        this->stg->size = resize;
    }
    
    void reserve(const ulong_t &resere){
        if (resere > this->capacity()){
            this->stg->capacity = resere;
            
        }
    }
    
    vector &push_back(const VecTe &val){
        this->AddItem(val);
        return *this;
    }
    
    void AddItem(const VecTe &val){
        if (this->IsStorage()){
            if (!this->IsStorageData()){
                this->stg->data      = this->InitializeArray(1, val, true);
                this->stg->size     += 1;
                this->stg->capacity += 1;
            }
            else{
                if (this->size() +1 > this->capacity()){
                    this->stg->capacity += 1;
                }
                
                this->IncreaseDataVolume(&this->stg->data, this->size(), this->size() +1, this->capacity());
                std::construct_at<VecTe>(this->stg->data + this->size(), std::move(val));
                this->stg->size += 1;
            }
        }
    }
    
private:
    Storage *CreateStorage(const data_p_t data, const ulong_t sz, const ulong_t cap){
        Storage *p_stg = static_cast<Storage*>(std::malloc(sizeof(Storage)));
        // This feature is optional since the Storage structure is trivial, but this exception only applies to compilers that support the C20 standard and later.
        std::construct_at<Storage>(p_stg, data, sz, cap); //
        return p_stg;
    }
    
    data_p_t InitializeArray(const ulong_t &sizem, const VecTe &val, const bool is_constr){
        if (sizem > 0){
            data_p_t data  = nullptr;
            ulong_t  align = alignof(VecTe);
            ulong_t  sizef = sizeof(VecTe);
            
            if (align <= alignof(std::max_align_t)){
                data = static_cast<data_p_t>(std::malloc(sizef * sizem));
            }else{
                if (sizef % align != 0){
                    sizef += align - (sizef % align);
                }
                data = static_cast<data_p_t>(std::aligned_alloc(align, sizef));
            }
            
            for (ulong_t i = 0; i < sizem && is_constr; i++){
                std::construct_at<VecTe>(data + i, val);
            }
            return data;
        }
        return nullptr;
    }
    
    void IncreaseDataVolume(data_p_t *p_data, const ulong_t &old_size, const ulong_t &new_size, const ulong_t &cap){
        data_p_t data   = nullptr;
        bool is_realloc = true;
        
        if constexpr (std::is_trivially_copyable_v<VecTe> && alignof(VecTe) <= alignof(std::max_align_t)){
            data = reinterpret_cast<data_p_t>(std::realloc(*p_data, sizeof(VecTe) *cap));
        }else{
            data = this->InitializeArray(cap, {}, false);
            is_realloc = false;
        }
        
        if (data == nullptr){
            return;
        }
        else{
            if (!is_realloc){
                for (ulong_t i = 0; i < old_size; i++){
                    std::construct_at<VecTe>(data +i, std::move(*((*p_data) +i)));
                }
                this->RemoveArray(*p_data, old_size);
            }
        }
        *p_data = data;
    }
    
    void RemoveArray(data_p_t data, const ulong_t &size){
        if (data != nullptr && size > 0){
            for (ulong_t i = 0; i < size; i++){
                std::destroy_at(data + i);
            }
            std::free(data);
            this->stg->data = nullptr;
        }
    }
    bool IsStorage() const{
        return this->stg != nullptr;
    }
    
    bool IsStorageData() const{
        return this->stg->data != nullptr;
    }
    
};

}
#endif /* dde_vector_hpp */
