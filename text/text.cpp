#include "text.hpp"

dde::text::text() : set(0, dde::text::STR_CAPACITY), stg(new storage(new char[dde::text::STR_CAPACITY +1], 0, dde::text::STR_CAPACITY, nullptr, nullptr)){}

dde::text::ulong_t dde::text::GetStrLen(c_char_p_t str) const{
    ulong_t size = 0;
    if (str != nullptr){
        while (str[size] != '\0') {
            size++;
        }
    }
    return size;
}

dde::text::ulong_t dde::text::IncreaseCapacity(const dde::text::ulong_t &m_cap, const dde::text::ulong_t &size){
    ulong_t capacity(m_cap);
    
    if (capacity < size){
        while (capacity < size) {
            capacity *= dde::text::STR_INCREASE;
        }
    }
    
    return capacity;
}

dde::text::char_p_t dde::text::GetNewArr(const dde::text::ulong_t &size){
    return new char[size]{};
}

void dde::text::CopyStrToArr(dde::text::c_char_p_t str, dde::text::char_p_t arr, const dde::text::ulong_t &size){
    if (!str || !arr || !size){
        return;
    }
    else{
        for (dde::text::ulong_t i = 0; i < size; i++){
            arr[i] = str[i];
        }
    }
}

dde::text &dde::text::AddStr(dde::text::c_char_p_t str, text &obj){
    if (str != nullptr){
        ulong_t size  = obj.GetStrLen(str);
        ulong_t o_res = obj.stg->reserve - obj.stg->size;
        ulong_t o_bal = 0;
        
        if (o_res > size){ 
            o_bal = size;
        }
        else{
            o_bal = o_res;
        }
        
        if (o_res > 0){
            obj.CopyStrToArr(str, obj.stg->arr + obj.stg->size, o_bal);
            obj.stg->size += o_bal;
            obj.set.size  += o_bal;
        }
        
        if (o_bal < size){
            ulong_t cap = obj.IncreaseCapacity(obj.stg->reserve, obj.stg->size + size);
            
            obj.stg->next = new storage(new char[cap +1], size - o_res, cap, obj.stg, nullptr);
            obj.stg = obj.stg->next;
            obj.CopyStrToArr(str + o_res, obj.stg->arr, (size - o_res));
        }
    }
    return *this;
}
