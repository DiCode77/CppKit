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

dde::string::string(const dde::string &in, const dde::ulong_t &pos, const dde::ulong_t &size) : dde::string::string(){
    this->set(in, pos, size);
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

dde::ulong_t dde::string::capacity() const{
    return this->stg->capacity;
}

dde::char_ref_t dde::string::at(ulong_t pos){
    return *(this->stg->arr + pos);
}

bool dde::string::empty() const{
    if (this->stg == nullptr)
        return true;
    return !(this->stg->size > 0);
}

dde::string &dde::string::set(const dde::string &str){
    if (this->stg->size != 0 || this->stg->capacity < str.stg->capacity){
        this->stg->capacity = this->IncreaseCapacity(this->stg->capacity, str.stg->size);
    }
    
    this->clear();
    this->CopyStrToArr(str.stg->arr, this->stg->arr, str.stg->size +1);
    this->stg->size = str.stg->size;
    
    return *this;
}

dde::string &dde::string::set(const dde::string &str, const dde::ulong_t &pos, const dde::ulong_t &len){
    if (!str.empty() && len > this->capacity()){
        this->stg->capacity = this->IncreaseCapacity(this->stg->capacity, len);
    }
    
    this->clear();
    this->CopyStrToArr(str.stg->arr + pos, this->stg->arr, len);
    this->stg->size = len;
    
    return *this;
}

dde::string &dde::string::clear(){
    delete [] this->stg->arr;
    this->stg->arr = this->GetNewArr(this->stg->capacity +1);
    this->stg->size = 0;
    
    return *this;
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

dde::string &dde::string::append(const dde::string &str){
    if (!str.empty()){
        
        ulong_t m_size = this->stg->size + str.stg->size;
        if (m_size > this->stg->capacity){
            this->stg->capacity = this->IncreaseCapacity(this->stg->capacity, m_size);
            
            char_p_t _arr = this->stg->arr;
            this->stg->arr = this->GetNewArr(this->stg->capacity +1);
            
            this->CopyStrToArr(_arr, this->stg->arr, this->stg->size);
            this->CopyStrToArr(str.stg->arr, this->stg->arr + this->stg->size, str.stg->size +1);
            
            this->stg->size += str.stg->size;
            
            delete [] _arr;
        }
        else{
            this->CopyStrToArr(str.stg->arr, this->stg->arr + this->stg->size, str.stg->size +1);
            this->stg->size += str.stg->size;
        }
    }
    
    return *this;
}

dde::string &dde::string::append(c_char_p_t str){
    if (str != nullptr){
        ulong_t s_size = this->GetStrlen(str);
        ulong_t m_size = s_size + this->stg->size;
        
        if (m_size > this->stg->capacity){
            this->stg->capacity = this->IncreaseCapacity(this->stg->capacity, m_size);
            
            char_p_t _arr = this->stg->arr;
            this->stg->arr = this->GetNewArr(this->stg->capacity +1);
            
            this->CopyStrToArr(_arr, this->stg->arr, this->stg->size);
            this->CopyStrToArr(str, this->stg->arr + this->stg->size, s_size +1);
            
            this->stg->size += s_size;
            
            delete [] _arr;
        }
        else{
            this->CopyStrToArr(str, this->stg->arr + this->stg->size, s_size +1);
            this->stg->size += s_size;
        }
        
    }
    
    return *this;
}

dde::string &dde::string::append(const dde::string &str, const dde::ulong_t &size){
    return this->append(dde::string(str, 0, size));
}

dde::string &dde::string::append(dde::c_char_p_t str, const dde::ulong_t &size){
    return this->append(dde::string(str, 0, size));
}

dde::string &dde::string::append(const dde::ulong_t &wle, const dde::string &str){
    for (dde::ulong_t i = 0; i < wle; i++){
        this->append(str);
    }
    return *this;
}

dde::string &dde::string::append(const dde::ulong_t &wle, dde::c_char_p_t str){
    for (dde::ulong_t i = 0; i < wle; i++){
        this->append(str);
    }
    return *this;
}

dde::ulong_t dde::string::find(const dde::string &str) const{
    return this->Find(str);
}

dde::ulong_t dde::string::rfind(const dde::string &str) const{
    return this->RFind(str);
}

dde::string &dde::string::operator+= (const dde::string &str){
    return this->append(str);
}

dde::string &dde::string::operator+= (dde::c_char_p_t str){
    return this->append(str);
}

dde::string dde::string::operator+ (const dde::string &str) const{
    return dde::string(*this).append(str);
}

bool dde::string::operator== (const dde::string &str) const{
    return this->CompareString(*this, str);
}

bool dde::string::operator== (dde::c_char_p_t str) const{
    return this->CompareString(*this, dde::string(str));
}

bool dde::string::operator!= (const dde::string &str) const{
    return !this->CompareString(*this, str);
}

bool dde::string::operator!= (dde::c_char_p_t c_str) const{
    return !this->CompareString(*this, dde::string(c_str));
}

bool dde::operator== (dde::c_char_p_t c_str, const dde::string &str){
    return str == c_str;
}

bool dde::operator!= (dde::c_char_p_t c_str, const dde::string &str){
    return str != c_str;
}

dde::ulong_t dde::string::GetStrlen(dde::c_char_p_t str) const{
    dde::ulong_t size = 0;
    while (str[size] != '\0'){
        size++;
    }
    return size;
}

dde::char_p_t dde::string::GetNewArr(const dde::ulong_t &size){
    return new char[size]{};
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

bool dde::string::CompareString(const dde::string &in1, const dde::string &in2) const{
    if (!in1.empty() && !in2.empty()){
        if (in1.size() == in2.size()){
            for (dde::ulong_t i = 0; i < in1.size(); i++){
                if (in1.stg->arr[i] != in2.stg->arr[i]){
                    return false;
                }
            }
        }
        else{
            return false;
        }
    }
    return true;
}

dde::ulong_t dde::string::Find(const dde::string &str) const{
    if (!str.empty()){
        for (dde::ulong_t i = 0; i < this->size(); i++){
            if (this->stg->arr[i] == str.stg->arr[0]){
                dde::ulong_t seq = 0;
                for (dde::ulong_t j = 0; j < str.size() && i + j < this->size(); j++){
                    if (this->stg->arr[i + j] == str.stg->arr[j]){
                        if (++seq == str.size()){
                            return i;
                        }
                    }else{
                        break;
                    }
                }
            }
        }
    }
    return dde::npos;
}

dde::ulong_t dde::string::RFind(const dde::string &str) const{
    if (!str.empty()){
        for (dde::ulong_t i = this->size() -1; i > 0; i--){
            if (this->stg->arr[i] == str.stg->arr[str.size() -1]){
                dde::ulong_t seq = 0;
                for (dde::ulong_t j = 0; j < str.size(); j++){
                    if (this->stg->arr[i - j] == str.stg->arr[(str.size() -1) - j]){
                        if (++seq == str.size()){
                            return i - (str.size() -1);
                        }
                    }else{
                        break;
                    }
                }
            }
        }
    }
    return dde::npos;
}
