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
class list{
    S_DATA<Tp>   *p_data = nullptr;
    unsigned long p_size = 0;
    std::vector<S_DATA<Tp>*> p_Veclist;
public:
    
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
    
    S_DATA<Tp> *begin(){
        return this->p_Veclist.at(0);
    }
    
    S_DATA<Tp> *end(){
        return this->p_Veclist.at(this->p_Veclist.size() -1);
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
            this->UpdatePointersList(this->p_data);
        }
    }
    
    void UpdatePointersList(S_DATA<Tp> *data){
        if (data != nullptr){
            if (!this->p_Veclist.empty()){
                this->p_Veclist.clear();
            }
            
            S_DATA<Tp> *buff = data;
            
            this->p_Veclist.push_back(data);
            for (int i = 0; i < this->p_size; i++){
                if (this->p_Veclist[i]->next != nullptr){
                    this->p_Veclist.push_back(this->p_Veclist[i]->next);
                }
                else{
                    break;
                }
            }
        }
    }
};

}

#endif /* list_hpp */
