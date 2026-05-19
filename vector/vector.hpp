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
#include <type_traits>

namespace dde {

// This check is necessary because the code uses “implicit-lifetime types”, which have been supported since the 20th standard.
#if __cplusplus >= 202002L
constexpr bool IMPLICT_LIFETIME_TYPE = true;
#else
constexpr bool IMPLICT_LIFETIME_TYPE = false;
#endif

template <typename VecTe>
class vector{
public:
    static constexpr const char *VEC_VERSION = "0.0.0-7b";
    
    using data_p_t = VecTe*;
    using ulong_t  = unsigned long;
    using c_vect_t = const VecTe;
private:
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
    vector();
    explicit vector(const ulong_t&);
    explicit vector(const ulong_t&, const VecTe&);
    
    vector(std::initializer_list<VecTe>);
    vector(const vector<VecTe>&);
    vector(vector<VecTe>&&) noexcept;
    ~vector();
    
    bool      empty() const;
    ulong_t   size() const;
    ulong_t   capacity() const;
    VecTe&    at(const ulong_t&);
    c_vect_t& at(const ulong_t&) const;
    data_p_t  data();
    VecTe&    front();
    VecTe&    back();
    ulong_t   max_size() const;
    void      clear();
    
    void resize(const ulong_t&, const VecTe& = {});
    void reserve(const ulong_t&);
    
    void push_back(const VecTe&);
    void push_back(VecTe&&) noexcept;
    template <typename... Te> void emplace_back(Te &&...t);
    void pop_back();
    
    void append_list(const std::initializer_list<VecTe>&);
    template <typename ...Te> requires ((... && std::is_convertible_v<Te, VecTe>) && sizeof...(Te) > 0)
    void append_va(Te &&...args);
    
    void insert(const ulong_t&, const VecTe&);
    
    void swap(vector<VecTe>&);
    
    VecTe  &operator[] (const ulong_t&);
    vector &operator= (const vector<VecTe>&);
    vector &operator= (vector<VecTe>&&) noexcept;
    vector &operator= (const std::initializer_list<VecTe>&);
    
private:
    Storage *CreateStorage(const data_p_t, const ulong_t, const ulong_t);
    template <typename Te> bool AppendToTheArray(data_p_t, const ulong_t&, Te&&);
    std::pair<data_p_t, design> AllocateMemory(data_p_t, const ulong_t&);
    ulong_t GrowCapacity(const ulong_t&, const ulong_t &);
    void RemoveArray(data_p_t*, const ulong_t&);
    void DestroyArray(data_p_t, const ulong_t&);
    void Destroy();
    inline bool IsStorage() const;
    inline bool IsStorageData() const;
};

}

#include "vector.inl"
#endif /* dde_vector_hpp */
