#include "string.hpp"

dde::string::string(){
    this->stg.arr      = nullptr;
    this->stg.size     = 0;
}

dde::string::string(dde::c_char_p_t str) : dde::string::string(){
    if (str == nullptr){
        return;
    }
    else{
        this->stg.size = GetStrlen(str);
        this->stg.arr = new char[this->stg.size +1];
        this->CopyStrToArr(str, this->stg.arr, this->stg.size +1);
    }
}

dde::string::string(const dde::string &in) : dde::string::string(){
    this->stg.size = in.stg.size;
    this->stg.arr = new char[this->stg.size +1];
    this->CopyStrToArr(in.stg.arr, this->stg.arr, this->stg.size +1);
}


dde::string::string(dde::string &&in) : dde::string::string(){
    this->stg.arr = in.stg.arr;
    this->stg.size = in.stg.size;
    
    in.stg.arr = nullptr;
    in.stg.size = 0;
}

dde::string::~string(){
    delete [] this->stg.arr;
}

dde::c_char_p_t dde::string::c_str(){
    return this->stg.arr;
}

dde::ulong_t dde::string::size() const{
    return this->stg.size;
}

dde::char_ref_t dde::string::at(ulong_t pos) const{
    return *(this->stg.arr + pos);
}

std::ostream& dde::operator<< (std::ostream &out, const dde::string &str){
    return out << str.stg.arr;
}

dde::char_ref_t dde::string::operator[] (dde::ulong_t pos){
    return *(this->stg.arr + pos);
}

dde::ulong_t dde::string::GetStrlen(dde::c_char_p_t str){
    dde::ulong_t size = 0;
    while (str[size] != '\0'){
        size++;
    }
    return size;
}

void dde::string::CopyStrToArr(dde::c_char_p_t str, dde::char_p_t arr, dde::ulong_t in_size){
    if (str == nullptr || arr == nullptr){
        return;
    }
    else{
        for (dde::ulong_t i = 0; i < in_size; i++){
            arr[i] = str[i];
        }
    }
}
