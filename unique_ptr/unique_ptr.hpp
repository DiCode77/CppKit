//
//  unique_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef unique_ptr_hpp
#define unique_ptr_hpp

namespace dde{

template<typename Te>
class unique_ptr{
    Te *p_data;
public:
    unique_ptr() : p_data(nullptr){}
    unique_ptr(Te *te) : p_data(te){}
    
    ~unique_ptr(){
        delete this->p_data;
        this->p_data = nullptr;
    }
};

};

#endif /* unique_ptr_hpp */
