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
constexpr ulong_t     npos         = ~0;

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
    string(const string&, const ulong_t&, const ulong_t&);
    string(string&&) noexcept;
    ~string();
    
    c_char_p_t c_str() const;
    ulong_t    size() const;
    ulong_t    capacity() const;
    char_ref_t at(ulong_t);
    bool       empty() const;
    
    string &set(const string&);
    string &set(const string&, const ulong_t&, const ulong_t&);
    string &clear();
    
    friend std::ostream& operator<< (std::ostream&, const string&);
    
    char_ref_t operator[] (const ulong_t&);
    string&    operator=  (const string&);
    string&    operator=  (string&&) noexcept;
    
    string &append(const string&);
    string &append(c_char_p_t);
    string &append(const string&, const ulong_t&);
    string &append(c_char_p_t, const ulong_t&);
    string &append(const ulong_t&, const string&);
    string &append(const ulong_t&, c_char_p_t);
    
    ulong_t find(const string&) const;
    ulong_t rfind(const string&) const;
    
    string &operator+= (const string&);
    string &operator+= (c_char_p_t);
    
    string operator+ (const string&) const;
    
    bool operator== (const string&) const;
    bool operator== (c_char_p_t) const;
    bool operator!= (const string&) const;
    bool operator!= (c_char_p_t) const;
    
    friend bool operator== (c_char_p_t, const string&);
    friend bool operator!= (c_char_p_t, const string&);
//    
private:
    ulong_t GetStrlen(c_char_p_t) const;
    char_p_t GetNewArr(const ulong_t&);
    void CopyStrToArr(c_char_p_t, char_p_t, const ulong_t&);
    ulong_t IncreaseCapacity(const ulong_t&, const ulong_t&);
    void Destroy();
    bool CompareString(const string&, const string&) const;
    ulong_t Find(const dde::string&) const;
    ulong_t RFind(const string&) const;
};

};

#endif /* string_hpp */
