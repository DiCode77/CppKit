//
//  text.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef text_hpp
#define text_hpp

/*
 This class is a test class
*/

namespace dde{
class text{
    using ulong_t    = unsigned long;
    using c_char_p_t = const char*;
    using char_p_t   = char*;
    using char_pp_t  = char**;
    
    static constexpr ulong_t     STR_CAPACITY = 35;
    static constexpr ulong_t     STR_INCREASE = 2;
    static constexpr const char *STR_VERSION  = "0.0.1";
    static constexpr ulong_t     npos         = ~0;
    
    struct{
        ulong_t size;
        ulong_t capacity;
    } set;
    
    struct storage{
        char_p_t  arr;
        ulong_t   size;
        ulong_t   reserve;
        storage   *back;
        storage   *next;
    } *stg;
    
public:
    text();
    text(c_char_p_t str) : text(){
        if (str == nullptr){
            return;
        }
        else{
            ulong_t siz = this->GetStrLen(str);
            ulong_t cap = this->IncreaseCapacity(STR_CAPACITY, siz);
            
            if (cap > STR_CAPACITY){
                delete [] this->stg->arr;
                this->stg->arr = this->GetNewArr(cap +1);
                this->stg->size    = siz;
                this->stg->reserve = cap;
            }
            else{
                this->stg->size    = siz;
                this->stg->reserve = cap;
            }
            
            this->CopyStrToArr(str, this->stg->arr, siz +1);
            this->set.size += siz;
            this->set.capacity = cap;
        }
    }
    
    ulong_t size() const{
        return this->set.size;
    }
    
    bool empty() const{
        return !(this->set.size > 0);
    }
    
    text &append(const text &obj){
        return this->AddStr(obj.stg->arr, *this);
    }
    
    text &append(c_char_p_t str){
        return this->AddStr(str, *this);
    }
    
    
private:
    ulong_t GetStrLen(c_char_p_t) const;
    ulong_t IncreaseCapacity(const ulong_t&, const ulong_t&);
    char_p_t GetNewArr(const ulong_t&);
    void CopyStrToArr(c_char_p_t, char_p_t, const ulong_t&);
    text &AddStr(c_char_p_t, text&);
};


};

#endif /* text_hpp */
