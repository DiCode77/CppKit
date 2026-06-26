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

template <typename>
class Iterator;

template <typename VecTe>
class vector{
public:
    static constexpr const char *VEC_VERSION = "0.0.1";
    
    using data_p_t = VecTe*;
    using ulong_t  = unsigned long;
    using c_vect_t = const VecTe;
    using value_t  =  VecTe;
    using iterator = Iterator<VecTe>;
    using const_iterator = const Iterator<VecTe>;
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
    vector&   clear();
    
    vector &resize(const ulong_t&, const VecTe& = {});
    vector &reserve(const ulong_t&);
    
    vector &push_back(const VecTe&);
    vector &push_back(VecTe&&) noexcept;
    template <typename... Te> vector &emplace_back(Te &&...t);
    vector &pop_back();
    
    vector &append_list(const std::initializer_list<VecTe>&);
    template <typename ...Te> requires ((... && std::is_convertible_v<Te, VecTe>) && sizeof...(Te) > 0)
    vector &append_va(Te &&...args);
    
    // As for the insert methods, this is an experimental implementation that is currently being tested.
    vector &insert(const ulong_t&, const VecTe&);
    vector &insert(const ulong_t&, const std::initializer_list<VecTe>&);
    
    vector &swap(vector<VecTe>&);
    
    // These are methods for completely redefining a vector with new values, while preserving its capacity.
    vector &assign(const ulong_t&);
    vector &assign(const ulong_t&, const VecTe&);
    vector &assign(const std::initializer_list<VecTe>&);
    vector &assign(const vector&);
    vector &assign(vector&&) noexcept;
    
    template <typename Te>
    vector &run_func(Te&&);
    
    vector &erase(const ulong_t&);
    vector &erase(const ulong_t&, const ulong_t&);
    
    VecTe  &operator[] (const ulong_t&);
    vector &operator= (const vector<VecTe>&);
    vector &operator= (vector<VecTe>&&) noexcept;
    vector &operator= (const std::initializer_list<VecTe>&);
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    
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

template <typename Te>
class Iterator{
    Te *m_data;
public:
    Iterator();
    Iterator(Te*);
    Iterator(const Iterator&);
    Iterator(Iterator&&);
    ~Iterator();
    
    bool operator!= (const Iterator&);
    bool operator== (const Iterator&);
    
    Iterator &operator++ ();
    Iterator operator++ (int);
    
    Iterator &operator-- ();
    Iterator operator-- (int);

    Te &operator* ();
    Te &operator-> ();
    
    Te &get();
};

}

#include "vector.inl"
#endif /* dde_vector_hpp */
