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
    static constexpr const char *VEC_VERSION = "0.0.0-1b";
    
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
        this->stg->data     = this->InitializeArray(in_size_vec, {});
        this->stg->size     = in_size_vec;
        this->stg->capacity = in_size_vec;
    }
    
    explicit vector(const ulong_t &in_size_vec, const VecTe &t_val) : vector(){
        this->stg->data     = this->InitializeArray(in_size_vec, t_val);
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
    
    
private:
    Storage *CreateStorage(const data_p_t data, const ulong_t sz, const ulong_t cap){
        Storage *p_stg = static_cast<Storage*>(std::malloc(sizeof(Storage)));
        // This feature is optional since the Storage structure is trivial, but this exception only applies to compilers that support the C20 standard and later.
        std::construct_at<Storage>(p_stg, data, sz, cap); //
        return p_stg;
    }
    
    data_p_t InitializeArray(const ulong_t &size, const VecTe &val){
        if (size > 0){
            data_p_t data = static_cast<data_p_t>(std::malloc(sizeof(VecTe) * size));
            for (ulong_t i = 0; i < size; i++){
                std::construct_at<VecTe>(data + i, val);
            }
            return data;
        }
        return nullptr;
    }
    
    void RemoveArray(data_p_t data, const ulong_t &size){
        if (data != nullptr && size > 0){
            for (ulong_t i = 0; i < size; i++){
                std::destroy_at(data + i);
            }
            std::free(data);
        }
    }
};

}
#endif /* dde_vector_hpp */
