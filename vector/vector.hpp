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
#include <stdexcept>

// This check is necessary because the code uses “implicit-lifetime types”, which have been supported since the 20th standard.
#if __cplusplus >= 202002L
constexpr bool IMPLICT_LIFETIME_TYPE = true;
#else
constexpr bool IMPLICT_LIFETIME_TYPE = false;
#endif

namespace dde {

template <typename VecTe>
class vector{
    static constexpr const char *VEC_VERSION = "0.0.0-5b";
    
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
        this->resize(in_size_vec);
    }
    
    explicit vector(const ulong_t &in_size_vec, const VecTe &t_val) : vector(){
        this->resize(in_size_vec, t_val);
    }
    
    vector(std::initializer_list<VecTe> list) : vector(){
        this->reserve(this->GrowCapacity(list.size(), this->capacity()));
        for (auto it = list.begin(); it != list.end(); it++){
            this->push_back(*it);
        }
    }
    
    vector(const vector &l_val) : vector(){
        this->reserve(l_val.capacity());
        for (ulong_t i = 0; i < l_val.size(); i++){
            this->push_back(l_val.at(i));
        }
    }
    
    vector(vector &&r_val) noexcept : vector(){
        // It's just a test, but the thing is, this type of storage should always be there, so there's no point in creating it separately; it would be faster to just swap them.
        std::swap<Storage*>(this->stg, r_val.stg);
    }
    
    ~vector(){
        this->Destroy();
    }
    
    bool empty() const{
        if (this->stg == nullptr)
            return true;
        return !(this->stg->size > 0);
    }
    
    ulong_t size() const{
        if (this->stg == nullptr)
            return 0;
        return this->stg->size;
    }
    
    ulong_t capacity() const{
        if (this->stg == nullptr)
            return 0;
        return this->stg->capacity;
    }
    
    VecTe &at(const ulong_t &pos){
        if (pos >= this->size()){
            throw std::runtime_error("Termination due to an out-of-bounds error!");
        }
        return *(this->stg->data + pos);
    }
    
    const VecTe &at(const ulong_t &pos) const{
        if (pos >= this->size()){
            throw std::runtime_error("Termination due to an out-of-bounds error!");
        }
        return *(this->stg->data + pos);
    }
    
    void clear(){
        this->DestroyArray(this->stg->data, this->size());
        this->stg->size = 0;
    }
    
    void resize(const ulong_t &rsize, const VecTe &val = {}){
        if (rsize > this->size()){
            if (rsize >= this->capacity()){
                ulong_t n_cap = this->GrowCapacity(rsize, this->capacity());
                this->reserve(n_cap);
                
                // This is a test. I'm testing the code's execution speed and optimization.
                if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
                    if (val != VecTe{}){
                        for (ulong_t i = this->size(); i < rsize; i++){
                            *(this->stg->data +i) = val;
                        }
                    }else{
                        std::memset(reinterpret_cast<void*>(this->stg->data + this->size()), 0, sizeof(VecTe) * (rsize - this->size()));
                    }
                }else{
                    for (ulong_t i = this->size(); i < rsize; i++){
                        std::construct_at<VecTe>(this->stg->data +i, val);
                    }
                }
            }
            else{
                if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
                    if (val != VecTe{}){
                        for (ulong_t i = this->size(); i < rsize; i++){
                            *(this->stg->data +i) = val;
                        }
                    }else{
                        std::memset(reinterpret_cast<void*>(this->stg->data), 0, sizeof(VecTe) * (rsize - this->size()));
                    }
                }else{
                    for (ulong_t i = this->size(); i < rsize; i++){
                        std::construct_at<VecTe>(this->stg->data +i, val);
                    }
                }
            }
        }else{
            if constexpr (!std::is_trivially_destructible_v<VecTe>){
                for (ulong_t i = rsize; i < this->size(); i++){
                    std::destroy_at<VecTe>(this->stg->data +i);
                }
            }
            if (rsize != this->size()){
                std::memset(reinterpret_cast<void*>(this->stg->data + rsize), 0, sizeof(VecTe)  * (this->size() - rsize));
            }
        }
        this->stg->size = rsize;
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
    
    void push_back(const VecTe &val){
        if (this->size() +1 > this->capacity()){
            ulong_t cap = this->GrowCapacity(this->size() +1, this->capacity());
            this->reserve(cap);
        }
        
        if constexpr (std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
            *(this->stg->data + this->size()) = val;
        }else{
            std::construct_at<VecTe>(this->stg->data + this->size(), val);
        }
        
        this->stg->size++;
    }
    
    void pop_back(){
        if (this->size() > 0){
            this->DestroyArray(this->stg->data + this->size() -1, 1);
            this->stg->size--;
        }
    }
    
    VecTe &operator[] (const ulong_t &index){
        return this->at(index);
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
            
            if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe>){
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
    
    ulong_t GrowCapacity(const ulong_t &size, const ulong_t &cap){
        ulong_t n_cap = (cap == 0 ? 1 : cap);
        while (size >= n_cap) {
            if (n_cap > std::numeric_limits<ulong_t>::max() / 2){
                return std::numeric_limits<ulong_t>::max();
            }
            n_cap *= 2;
        }
        return n_cap;
    }
    
    void RemoveArray(data_p_t data, const ulong_t &size){
        if (data != nullptr){
            this->DestroyArray(data, size);
            std::free(data);
            this->stg->data = nullptr;
        }
    }
    
    void DestroyArray(data_p_t data, const ulong_t &size){
        if (data != nullptr){
            if constexpr (std::is_trivially_destructible_v<VecTe>){
                if (size > 0){
                    std::memset(reinterpret_cast<void*>(data), 0, sizeof(VecTe) * size);
                }
            }else{
                for (ulong_t i = 0; i < size; i++){
                    std::destroy_at(data + i);
                }
            }
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
    
    inline bool IsStorage() const{
        return this->stg != nullptr;
    }
    
    inline bool IsStorageData() const{
        return this->stg->data != nullptr;
    }
    
};

}
#endif /* dde_vector_hpp */
