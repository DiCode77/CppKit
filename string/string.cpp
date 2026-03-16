#include "string.hpp"

dde::string::string() : stg(new storage(this->GetNewArr(dde::STR_CAPACITY), 0, dde::STR_CAPACITY)){}

dde::string::string(dde::c_char_p_t str) : dde::string::string(){
    if (str == nullptr){
        return;
    }
    else{
        if (this->stg != nullptr){
            this->stg->size = this->GetStrlen(str);
            ulong_t cap = this->IncreaseCapacity(this->stg->capacity, this->stg->size);
            
            if (cap > this->stg->capacity){
                delete [] this->stg->arr;
                this->stg->arr = this->GetNewArr(cap +1);
                this->stg->capacity = cap;
            }
            this->CopyStrToArr(str, this->stg->arr, this->stg->size +1);
        }
    }
}

dde::string::string(const dde::string &in) : dde::string::string(){
    this->set(in);
}


dde::string::string(dde::string &&in) noexcept : dde::string::string(){
    this->Destroy();
    
    this->stg = in.stg;
    in.stg    = nullptr;
}

dde::string::~string(){
    this->Destroy();
}

dde::c_char_p_t dde::string::c_str() const{
    return this->stg->arr;
}

dde::ulong_t dde::string::size() const{
    return this->stg->size;
}

dde::char_ref_t dde::string::at(ulong_t pos) const{
    return *(this->stg->arr + pos);
}

bool dde::string::empty() const{
    if (this->stg == nullptr)
        return true;
    return !(this->stg->size > 0);
}

void dde::string::set(const string &str){
    if (this->stg->size != 0 || this->stg->capacity < str.stg->capacity){
        this->stg->capacity = this->IncreaseCapacity(this->stg->capacity, str.stg->size);
        this->clear();
    }
    
    this->CopyStrToArr(str.stg->arr, this->stg->arr, str.stg->size +1);
    this->stg->size = str.stg->size;
}

void dde::string::clear(){
    delete [] this->stg->arr;
    this->stg->arr = this->GetNewArr(this->stg->capacity +1);
    this->stg->size = 0;
}

std::ostream& dde::operator<< (std::ostream &out, const dde::string &str){
    return out << ((str.stg == nullptr) ? "" : str.stg->arr);
}

dde::char_ref_t dde::string::operator[] (const dde::ulong_t &pos){
    return *(this->stg->arr + pos);
}

dde::string& dde::string::operator= (const dde::string &sst){
    if (this != &sst)
        this->set(sst);
    return *this;
}

dde::string& dde::string::operator= (string &&r_obj) noexcept{
    this->Destroy();
    
    this->stg = r_obj.stg;
    r_obj.stg = nullptr;
    
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

dde::ulong_t dde::string::IncreaseCapacity(const ulong_t &m_cap, const ulong_t &size){
    ulong_t capacity(m_cap);
    
    if (capacity < size){
        while (capacity < size) {
            capacity *= dde::STR_INCREASE;
        }
    }
    
    return capacity;
}

void dde::string::Destroy(){
    if (this->stg != nullptr){
        delete [] this->stg->arr;
        delete this->stg;
        this->stg = nullptr;
    }
}
