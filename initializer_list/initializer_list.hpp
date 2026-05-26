//
//  initializer_list.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef initializer_list_hpp
#define initializer_list_hpp

namespace dde{

template <typename Te>
class initializer_list{
    using ulong_t  = unsigned long;
    using data_p_t = Te*;
    
    struct Storage{
        data_p_t data;
        ulong_t  size;
    } *stg;
    
public:
    initializer_list() : stg(new Storage(nullptr, 0)){}
    template <typename ...Args> requires (... && std::is_same_v<std::remove_cvref_t<Args>, Te>)
    initializer_list(Args &&...arg) : initializer_list(){
        this->stg->data = new Te[sizeof...(Args)]{};
        this->stg->size = sizeof...(Args);
        
        ulong_t i = 0;
        ((*(this->stg->data + (i++)) = std::forward<Args>(arg)), ...);
    }
    
    ~initializer_list(){
        if (this->stg->data != nullptr){
            delete [] this->stg->data;
        }
        delete this->stg;
    }
    
    Te &at(const ulong_t &index){
        return *(this->stg->data + index);
    }
    
    ulong_t size() const{
        return this->stg->size;
    }
};


}
#endif /* initializer_list_hpp */
