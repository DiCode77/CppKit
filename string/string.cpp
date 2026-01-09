#include "string.hpp"

dde::string::string(){
    this->stg.arr      = this->GetNewArr(dde::STR_CAPACITY);
    this->stg.size     = 0;
    this->stg.capacity = dde::STR_CAPACITY;
}

dde::string::string(dde::c_char_p_t str) : dde::string::string(){
    if (str == nullptr){
        return;
    }
    else{
        this->stg.size = GetStrlen(str);
        this->IncreaseCapacity(this->stg.size);
        this->stg.arr = this->GetNewArr(this->stg.capacity +1);
        this->CopyStrToArr(str, this->stg.arr, this->stg.size +1);
    }
}

dde::string::string(const dde::string &in) : dde::string::string(){
    this->stg.size = in.stg.size;
    this->stg.capacity = in.stg.capacity;
    this->stg.arr = this->GetNewArr(this->stg.capacity +1);
    this->CopyStrToArr(in.stg.arr, this->stg.arr, this->stg.size +1);
}


dde::string::string(dde::string &&in) : dde::string::string(){
    this->stg.arr = in.stg.arr;
    this->stg.size = in.stg.size;
    this->stg.capacity = in.stg.capacity;
    
    in.stg.arr = nullptr;
    in.stg.size = 0;
    in.stg.capacity = dde::STR_CAPACITY;
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

void dde::string::clear(){
    delete [] this->stg.arr;
    this->stg.arr = this->GetNewArr(this->stg.capacity +1);
    this->stg.size = 0;
}

std::ostream& dde::operator<< (std::ostream &out, const dde::string &str){
    return out << str.stg.arr;
}

dde::char_ref_t dde::string::operator[] (const dde::ulong_t &pos){
    return *(this->stg.arr + pos);
}

dde::string& dde::string::operator= (const dde::string &sst){
    if (this->stg.size != 0 || this->stg.capacity < sst.stg.capacity){
        this->IncreaseCapacity(sst.stg.size);
        this->clear();
    }
    
    this->CopyStrToArr(sst.stg.arr, this->stg.arr, sst.stg.size +1);
    this->stg.size = sst.stg.size;
    
    return *this;
}

dde::ulong_t dde::string::GetStrlen(dde::c_char_p_t str){
    dde::ulong_t size = 0;
    while (str[size] != '\0'){
        size++;
    }
    return size;
}

dde::char_p_t dde::string::GetNewArr(const ulong_t &size){
    return new char[size];
}

void dde::string::CopyStrToArr(dde::c_char_p_t str, dde::char_p_t arr, const dde::ulong_t &in_size){
    if (str == nullptr || arr == nullptr){
        return;
    }
    else{
        for (dde::ulong_t i = 0; i < in_size; i++){
            arr[i] = str[i];
        }
    }
}

void dde::string::IncreaseCapacity(const ulong_t &size){
    if (this->stg.capacity < size){
        while (this->stg.capacity < size) {
            this->stg.capacity *= dde::STR_INCREASE;
        }
    }
}
