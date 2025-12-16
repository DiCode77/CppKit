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
    
    S_DATA() : back(this), next(this), val(Tp{}){}
    S_DATA(S_DATA *dat1, S_DATA *dat2, Tp Tdat = Tp{}) : back(dat1), next(dat2), val(Tdat){
        this->back->next = this;
        this->next->back = this;
    }
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
    
    void operator-- (int a){
        if (this->p_data != nullptr){
            this->p_data = this->p_data->back;
        }
    }
    
    bool operator!= (const Iterator<Tp> &it){
        return this->p_data != it.p_data;
    }
    
    S_DATA<Tp> **get(){
        return &this->p_data;
    }
};

template<typename Tp>
class list{
public:
    using iterator   = Iterator<Tp>;
    using ListStruct = S_DATA<Tp>;
    using u_long     = unsigned long;
    
private:
    ListStruct   *p_data      = nullptr;
    u_long        p_size      = 0;
    
public:
    list(){
        this->p_data = new ListStruct();
        this->p_size = 0;
    }
    
    list(std::initializer_list<Tp> _list) : list(){
        this->p_size = _list.size();
        this->InitList(_list);
    }
    
    ~list(){
        ListStruct *dat_next = this->p_data->next;
        this->p_data->back = nullptr;
        this->p_data->next = nullptr;
        
        while(dat_next != nullptr){
            ListStruct *buff = dat_next->next;
            delete dat_next;
            dat_next = buff;
        }
    }
    
    void insert(iterator it, const Tp &val){
        ListStruct *old_list = (*it.get())->back;
        ListStruct *new_list = new ListStruct();
        new_list->val = val;
        new_list->next = *it.get();
        new_list->next->back = new_list;
        new_list->back = old_list;
        old_list->next = new_list;
    }
    
    void push_back(Tp t){
        new ListStruct(this->p_data->back, this->p_data, t);
    }
    
    iterator begin(){
        return this->p_data->next;
    }
    
    iterator end(){
        return this->p_data;
    }
    
private:
    void InitList(std::initializer_list<Tp> &list){
        if (this->p_data != nullptr && list.size() != 0){
            ListStruct *listBuff = this->p_data;
            std::ranges::for_each(list.begin(), list.end(), [this, &listBuff](Tp t){
                listBuff->next = new ListStruct(listBuff, this->p_data, t);
                listBuff = listBuff->next;
            });
        }
    }
    
    S_DATA<Tp> *Jump(unsigned long pos){
        ListStruct *data = this->p_data;
        for (unsigned long i = 0; i < ((pos > this->p_size) ? this->p_size : pos); i++){
            data = data->next;
        }
        return data;
    }
};

template<typename Tp>
void advance(Iterator<Tp> &it, int n){
    S_DATA<Tp> *data = *it.get();
    for (int i = 0; i < n; i++){
        data = data->next;
    }
    
    *it.get() = data;
}

}

#endif /* list_hpp */
