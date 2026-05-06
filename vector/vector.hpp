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
#include <utility>

namespace dde {

template <typename VecTe>
class vector{
    static constexpr const char *VEC_VERSION = "0.0.0-3b";
    
    using data_p_t = VecTe*;
    using ulong_t  = unsigned long;
    
    struct Storage{
        data_p_t data;
        ulong_t  size;
        ulong_t  capacity;
    } *stg;
    
    enum class design{
        created,
        copy,
        null
    };
    
public:
    vector() : stg(this->CreateStorage(nullptr, 0, 0)){}
    explicit vector(const ulong_t &in_size_vec) : vector(){
    }
    
    explicit vector(const ulong_t &in_size_vec, const VecTe &t_val) : vector(){
    }
    
    vector(std::initializer_list<VecTe> list) : vector(){}
    
    ~vector(){
        this->Destroy();
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
    
    void reserve(const ulong_t &rcap){
        if (rcap > this->capacity()){
            if (this->capacity() == 0){
                this->stg->data = this->AllocateMemory(this->stg->data, rcap).first;
            }
            else{
                auto r_pair = this->AllocateMemory(this->stg->data, rcap);
                if (r_pair.second == design::copy){
                    this->stg->data = r_pair.first;
                }
                else if (r_pair.second == design::created){
                    for (ulong_t i = 0; i < this->size(); i++){
                        std::construct_at<VecTe>(r_pair.first + i, std::move(*(this->stg->data + i)));
                    }
                    this->RemoveArray(this->stg->data, this->size());
                    this->stg->data = r_pair.first;
                }
                else{
                    return;
                }
            }
            this->stg->capacity = rcap;
        }
    }
    
    
private:
    Storage *CreateStorage(const data_p_t data, const ulong_t sz, const ulong_t cap){
        Storage *p_stg = static_cast<Storage*>(std::malloc(sizeof(Storage)));
        // This feature is optional since the Storage structure is trivial, but this exception only applies to compilers that support the C20 standard and later.
        std::construct_at<Storage>(p_stg, data, sz, cap); //
        return p_stg;
    }
    
    std::pair<data_p_t, design> AllocateMemory(data_p_t old_data, const ulong_t &cap){
        if (cap > 0){
            void    *data   = nullptr;
            ulong_t  align  = alignof(VecTe);
            ulong_t  sizef  = sizeof(VecTe) * cap;
            
            std::pair<data_p_t, design> r_pair;
            
            if constexpr(std::is_trivially_copyable_v<VecTe>){
                data = std::realloc(old_data, sizef);
                
                if (data == nullptr){
                    r_pair.second = design::null;
                }else{
                    r_pair.second = design::copy;
                }
                
            }else{
                if (align <= alignof(std::max_align_t)){
                    data = std::malloc(sizef);
                }else{
                    if (sizef % align != 0){
                        sizef += align - (sizef % align);
                    }
                    data = std::aligned_alloc(align, sizef);
                }
                
                if (data == nullptr){
                    r_pair.second = design::null;
                }else{
                    r_pair.second = design::created;
                }
            }
            
            r_pair.first  = reinterpret_cast<data_p_t>(data);
            
            return r_pair;
        }
        return { nullptr, design::null };
    }
    
    void RemoveArray(data_p_t data, const ulong_t &size){
        if (data != nullptr){
            if constexpr (!std::is_trivially_destructible_v<VecTe>){
                for (ulong_t i = 0; i < size; i++){
                    std::destroy_at(data + i);
                }
            }
            std::free(data);
            this->stg->data = nullptr;
        }
    }
    
    void Destroy(){
        if (this->IsStorage()){
            if (this->IsStorageData()){
                this->RemoveArray(this->stg->data, this->stg->size);
            }
            std::free(this->stg);
            this->stg = nullptr;
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
