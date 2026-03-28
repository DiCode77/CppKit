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
