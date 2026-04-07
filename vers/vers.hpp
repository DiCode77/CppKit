//
//  vers.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef vers_hpp
#define vers_hpp
#include <typeinfo>

namespace dde{

class vers{
    enum class TYPE{
        NONE,
        BOOL,
        CHAR,
        INT,
        LONG,
        DOUBLE
    };
    
    struct data{
        void *s;
        TYPE  t;
    } t_info;
    
public:
    vers() : t_info(nullptr, TYPE::NONE){}
    vers(auto ao) : vers(){
        this->t_info.t = this->DetermineType(ao);
    }
private:
    TYPE DetermineType(const auto &ao){
        if (typeid(ao) == typeid(bool)){
            return TYPE::BOOL;
        }else if (typeid(ao) == typeid(char)){
            return TYPE::CHAR;
        }else if (typeid(ao) == typeid(int)){
            return TYPE::INT;
        }else if (typeid(ao) == typeid(long)){
            return TYPE::LONG;
        }else if (typeid(ao) == typeid(double)){
            return TYPE::DOUBLE;
        }
        return TYPE::NONE;
    }
};


};
#endif /* vers_hpp */
