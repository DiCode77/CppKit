//
//  pair.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef pair_hpp
#define pair_hpp

namespace dde{

template <typename Te_f, typename Te_s>
class pair{
    struct Storage{
        Te_f first;
        Te_s second;
    } *stg;
public:
    pair();
    pair(const Te_f&, const Te_s&);
    pair(const pair&);
    pair(pair&&) noexcept;
    ~pair();
    
    Te_f &first();
    Te_s &second();
    pair &swap(pair &sw);
    
    pair &operator= (const pair&);
    pair &operator= (pair&&) noexcept;
};

#include "pair.inl"
}

#endif /* pair_hpp */
