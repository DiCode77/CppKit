//
//  string.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef string_hpp
#define string_hpp

namespace dde{
  
class string{
    char          *arr;
    unsigned long u_size;
public:
    string(){
        this->arr    = nullptr;
        this->u_size = 0;
    }
    
    string(const char *str) : string(){
        if (str == nullptr){
            return;
        }
        else{
            this->u_size = GetStrlen(str);
            this->arr = new char[this->u_size +1];
            this->CopyStrToArr(str, arr, this->u_size +1);
        }
    }
    
    const char *c_str(){
        return this->arr;
    }
    
private:
    unsigned long GetStrlen(const char *str){
        unsigned long size = 0;
        while (str[size] != '\0'){
            size++;
        }
        return size;
    }
    
    void CopyStrToArr(const char *str, char *arr, unsigned long in_size){
        if (str == nullptr || arr == nullptr){
            return;
        }
        else{
            for (unsigned long i = 0; i < in_size; i++){
                arr[i] = str[i];
            }
        }
    }
};

};

#endif /* string_hpp */
