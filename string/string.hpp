//
//  string.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef string_hpp
#define string_hpp
#include <ostream>

namespace dde{

using char_p_t   = char*;
using c_char_p_t = const char*;
using ulong_t    = unsigned long;
using char_ref_t = char&;

constexpr ulong_t     STR_CAPACITY = 35;
constexpr ulong_t     STR_INCREASE = 2;
constexpr const char *STR_VERSION  = "0.0.2";

class string{
    struct storage{
        char_p_t arr;
        ulong_t  size;
        ulong_t  capacity;
    } *stg;
public:
    string();
    string(c_char_p_t);
    string(const string&);
    string(string&&) noexcept;
    ~string();
    
    c_char_p_t c_str() const;
    ulong_t    size() const;
    char_ref_t at(ulong_t) const;
    bool       empty() const;
    
    void set(const string&);
    void clear();
    
    friend std::ostream& operator<< (std::ostream&, const string&);
    
    char_ref_t operator[] (const ulong_t&);
    string&    operator=  (const string&);
    string&    operator=  (string&&) noexcept;
    
private:
    ulong_t GetStrlen(c_char_p_t);
    char_p_t GetNewArr(const ulong_t&);
    void CopyStrToArr(c_char_p_t, char_p_t, const ulong_t&);
    ulong_t IncreaseCapacity(const ulong_t&, const ulong_t&);
    void Destroy();
};

};

#endif /* string_hpp */
