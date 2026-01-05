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
  
typedef struct Storage{
    char_p_t arr;
    ulong_t  size;
} Storage;

class string{
    Storage stg;
public:
    string();
    string(c_char_p_t);
    string(const string&);
    string(string&&);
    ~string();
    
    c_char_p_t c_str();
    ulong_t    size() const;
    char_ref_t at(ulong_t) const;
    
    friend std::ostream& operator<< (std::ostream&, const string&);
    
    char_ref_t operator[] (ulong_t);
    
private:
    ulong_t GetStrlen(c_char_p_t str);
    void CopyStrToArr(c_char_p_t str, char_p_t arr, ulong_t in_size);
};

};

#endif /* string_hpp */
