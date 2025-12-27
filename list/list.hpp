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

constexpr const char *LIST_VERSION = "0.0.1";

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
    // This is part of iterator traits, needed so that standard STL functions know that my iterator class is indeed an iterator, so it needs to be reported.
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = Tp;
    using difference_type   = std::ptrdiff_t;
    
    Iterator() = default;
    Iterator(S_DATA<Tp> *data){
        this->p_data = data;
    }

    Tp &operator* () const{
        return this->p_data->val;
    }
    
    Iterator operator++ (int){
        Iterator tmp = *this;
        if (this->p_data != nullptr){
            this->p_data = this->p_data->next;
        }
        return tmp;
    }
    
    Iterator &operator++ (){
        if (this->p_data != nullptr){
            this->p_data = this->p_data->next;
        }
        return *this;
    }
    
    Iterator operator-- (int a){
        Iterator tmp = *this;
        if (this->p_data != nullptr){
            this->p_data = this->p_data->back;
        }
        return tmp;
    }
    
    Iterator &operator-- (){
        if (this->p_data != nullptr){
            this->p_data = this->p_data->back;
        }
        return *this;
    }
    
    bool operator== (const Iterator<Tp> &it) const{
        return this->p_data == it.p_data;
    }
    
    bool operator!= (const Iterator<Tp> &it) const{
        return this->p_data != it.p_data;
    }
    
    S_DATA<Tp> *get() const{
        return this->p_data;
    }
};

template<typename Tp>
class list{
public:
    using iterator       = Iterator<Tp>;
    using const_iterator = const Iterator<Tp>;
    using ListStruct     = S_DATA<Tp>;
    using u_long         = unsigned long;
    
    
private:
    ListStruct   *p_data      = nullptr;
    u_long        p_size      = 0;
    
public:
    list(){
        this->p_data = this->CreateNode();
        this->p_size = 0;
    }
    
    list(const u_long size) : list(){
        for (u_long i = 0; i < size; i++){
            this->push_back(0);
        }
    }
    
    list(const u_long size, const Tp init_val) : list(){
        for (u_long i = 0; i < size; i++){
            this->push_back(init_val);
        }
    }
    
    list(std::initializer_list<Tp> _list) : list(){
        this->p_size = _list.size();
        this->InitList(_list);
    }
    
    list(const list &_list) : list(){
        std::ranges::for_each(_list.begin(), _list.end(), [this](Tp t){
            this->push_back(t);
        });
    }
    
    list(list && _list){
        this->p_data = _list.p_data;
        this->p_size = _list.p_size;
        
        _list.p_data = _list.CreateNode();
        _list.p_size = 0;
    }
    
    ~list(){
        this->ClearList();
    }
    
    void insert(iterator &it, const Tp &val){
        this->CreateNode(it.get()->back, it.get(), val);
        this->p_size++;
    }
    
    void push_front(const Tp &t){
        this->CreateNode(this->p_data, this->p_data->next, t);
        this->p_size++;
    }
    
    void push_back(const Tp &t){
        this->CreateNode(this->p_data->back, this->p_data, t);
        this->p_size++;
    }
    
    void push_back(const list &_list){
        std::ranges::for_each(_list.begin(), _list.end(), [this](Tp t){
            this->CreateNode(this->p_data->back, this->p_data, t);
            this->p_size++;
        });
    }
    
    void pop_front(){
        if (this->p_size == 0)
            return;
        this->EraseNode(this->p_data->next);
    }
    
    void pop_back(){
        if (this->p_size == 0)
            return;
        this->EraseNode(this->p_data->back);
    }
    
    iterator begin(){
        return this->p_data->next;
    }
    
    const_iterator begin() const {
        return this->p_data->next;
    }
    
    iterator end(){
        return this->p_data;
    }
    
    const_iterator end() const{
        return this->p_data;
    }
    
    u_long size() const{
        return this->p_size;
    }
    
    void clear(){
        this->ClearList();
        this->p_data = this->CreateNode();
    }
    
    void erase(iterator &it){
        this->EraseNode(it);
    }
    
    Tp &front(){
        return this->p_data->next->val;
    }
    
    Tp &back(){
        return this->p_data->back->val;
    }
    
    bool empty(){
        return !(this->p_data != this->p_data->next);
    }
    
    Tp &at(u_long pos){
        iterator it = this->begin();
        if (this->p_data != this->p_data->next){
            for (u_long i = 0; i < pos; i++){
                it++;
            }
        }
        return *it;
    }
    
private:
    void InitList(const std::initializer_list<Tp> &list){
        if (this->p_data != nullptr && list.size() != 0){
            ListStruct *listBuff = this->p_data;
            std::ranges::for_each(list.begin(), list.end(), [this, &listBuff](Tp t){
                listBuff->next = this->CreateNode(listBuff, this->p_data, t);
                listBuff = listBuff->next;
            });
        }
    }
    
    void ClearList(){
        ListStruct *list = this->p_data->next;
        this->p_data->back = nullptr;
        this->p_data->next = nullptr;
        this->p_size = 0;
        
        while (list != nullptr) {
            ListStruct *list_next = list->next;
            delete list;
            list = list_next;
        }
    }
    
    void EraseNode(iterator it){
        if (this->p_data != it.get()){
            ListStruct *node = it.get();
            node->back->next = node->next;
            node->next->back = node->back;
            delete node;
            this->p_size--;
        }
    }
    
    ListStruct *CreateNode(){
        return new ListStruct();
    }
    
    ListStruct *CreateNode(ListStruct *start, ListStruct *end, const Tp &t){
        return new ListStruct(start, end, t);
    }
};

template<typename Tp>
void advance(Iterator<Tp> &it, int n){
    for (int i = 0; i < n; ++i){
        ++it;
    }
}

}

#endif /* list_hpp */
