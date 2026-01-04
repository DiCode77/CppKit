#include "string.hpp"

dde::string::string(){
    this->arr    = nullptr;
    this->u_size = 0;
}

dde::string::string(dde::c_char_p_t str) : dde::string::string(){
    if (str == nullptr){
        return;
    }
    else{
        this->u_size = GetStrlen(str);
        this->arr = new char[this->u_size +1];
        this->CopyStrToArr(str, arr, this->u_size +1);
    }
}

dde::string::string(const dde::string &in) : dde::string::string(){
    this->u_size = in.u_size;
    this->arr = new char[this->u_size +1];
    this->CopyStrToArr(in.arr, this->arr, this->u_size +1);
}


dde::string::string(dde::string &&in) : dde::string::string(){
    this->arr = in.arr;
    this->u_size = in.u_size;
    
    in.arr = nullptr;
    in.u_size = 0;
}

dde::string::~string(){
    delete [] this->arr;
}

dde::c_char_p_t dde::string::c_str(){
    return this->arr;
}

dde::ulong_t dde::string::size() const{
    return this->u_size;
}

dde::char_ref_t dde::string::at(ulong_t pos) const{
    return *(this->arr + pos);
}

std::ostream& dde::operator<< (std::ostream &out, const dde::string &str){
    return out << str.arr;
}

dde::char_ref_t dde::string::operator[] (dde::ulong_t pos){
    return *(this->arr + pos);
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
