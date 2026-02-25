//
//  shared_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef shared_ptr_hpp
#define shared_ptr_hpp

namespace dde{

template <typename Te>
class shared_ptr{
    using ulong_t = unsigned long;
    
    ulong_t *strong;
    ulong_t *weak;
    Te      *data;
public:
    shared_ptr() : strong(nullptr), weak(nullptr), data(nullptr){}
    shared_ptr(Te *t) : strong(new ulong_t(1)), weak(new ulong_t(0)), data(t){}
    ~shared_ptr(){
        *this->strong -= (this->strong != nullptr) ? 1 : 0;
        
        if (!*this->strong){
            delete this->strong;
            delete this->weak;
            delete this->data;
        }
    }
};

}

#endif /* shared_ptr_hpp */
