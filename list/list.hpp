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
#include <vector>

namespace dde{

template<typename Tp>
struct S_DATA{
    Tp val;
    S_DATA *back = nullptr;
    S_DATA *next = nullptr;
};

template<typename Tp>
class Iterator{
    S_DATA<Tp> *p_data = nullptr;
public:
    
    Iterator() = default;
    Iterator(S_DATA<Tp> *data){
        this->p_data = data;
    }

    Tp &operator* (){
        return this->p_data->val;
    }
    
    void operator++ (int a){
        if (this->p_data != nullptr){
            this->p_data = this->p_data->next;
        }
    }
    
    bool operator!= (const Iterator<Tp> &it){
        return this->p_data != it.p_data;
    }
    
};

template<typename Tp>
class list{
    S_DATA<Tp>   *p_data = nullptr;
    unsigned long p_size = 0;
public:
    using iterator = Iterator<Tp>;
    
    list() = default;
    list(std::initializer_list<Tp> _list) : p_size(_list.size()), p_data(new S_DATA<Tp>()){
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
    
    iterator begin(){
        return this->p_data;
    }
    
    iterator end(){
        return this->Jump(this->p_size);
    }
    
private:
    void InitList(std::initializer_list<Tp> &list){
        if (this->p_data != nullptr && list.size() != 0){
            S_DATA<Tp> *buffer = this->p_data;
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
    
    S_DATA<Tp> *Jump(unsigned long pos){
        S_DATA<Tp> *data = this->p_data;
        for (unsigned long i = 0; i < ((pos > this->p_size) ? this->p_size : pos); i++){
            data = data->next;
        }
        return data;
    }
};

}

#endif /* list_hpp */
