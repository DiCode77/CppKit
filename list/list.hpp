//
//  list.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef list_hpp
#define list_hpp
#include <initializer_list>
#include <ranges>

namespace dde{

template<typename Tp>
struct S_DATA{
    Tp val;
    S_DATA *back;
    S_DATA *next;
};

template<typename Tp>
class list{
    S_DATA<Tp>   *p_data;
    unsigned long p_size;
public:
    
    list(){
        this->p_data = new S_DATA<Tp>;
        this->p_size = 0;
    }
    
    list(std::initializer_list<Tp> _list) : list(){
        this->p_size = _list.size();
        this->InitList(_list);
    }
    
    ~list(){
        S_DATA<Tp> *buffer = this->p_data;
        while(buffer != nullptr){
            S_DATA<Tp> *buff = buffer->next;
            delete buffer;
            buffer = buff;
        }
    }
    
private:
    void InitList(std::initializer_list<Tp> &list){
        S_DATA<Tp> *buffer = this->p_data;
        if (buffer != nullptr){
            unsigned long size = 0;
            std::ranges::for_each(list.begin(), list.end(), [&buffer, &list, &size](Tp t){
                size++;
                buffer->val = t;
                if (size < list.size()){
                    buffer->next = new S_DATA<Tp>{};
                    buffer->next->back = buffer;
                    buffer = buffer->next;
                }
            });
        }
    }
};

}

#endif /* list_hpp */
