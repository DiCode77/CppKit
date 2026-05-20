//
//  vector.inl
//  CppKit
//
//  Created by DiCode77.
//
// This file contains the implementation of the template methods for the dde::vector class.

template <typename VecTe>
dde::vector<VecTe>::vector() : stg(this->CreateStorage(nullptr, 0, 0)){}

template <typename VecTe>
dde::vector<VecTe>::vector(const dde::vector<VecTe>::ulong_t &in_size_vec) : dde::vector<VecTe>::vector(){
    this->resize(in_size_vec);
}

template <typename VecTe>
dde::vector<VecTe>::vector(const dde::vector<VecTe>::ulong_t &in_size_vec, const VecTe &t_val) : dde::vector<VecTe>::vector(){
    this->resize(in_size_vec, t_val);
}

template <typename VecTe>
dde::vector<VecTe>::vector(std::initializer_list<VecTe> list) : dde::vector<VecTe>::vector(){
    this->reserve(this->GrowCapacity(list.size(), this->capacity()));
    for (auto it = list.begin(); it != list.end(); it++){
        this->push_back(*it);
    }
}

template <typename VecTe>
dde::vector<VecTe>::vector(const vector<VecTe> &l_val) : dde::vector<VecTe>::vector(){
    this->reserve(l_val.capacity());
    for (ulong_t i = 0; i < l_val.size(); i++){
        this->push_back(l_val.at(i));
    }
}

template <typename VecTe>
dde::vector<VecTe>::vector(vector<VecTe> &&r_val) noexcept : dde::vector<VecTe>::vector(){
    // It's just a test, but the thing is, this type of storage should always be there, so there's no point in creating it separately; it would be faster to just swap them.
    std::swap<Storage*>(this->stg, r_val.stg);
}

template <typename VecTe>
dde::vector<VecTe>::~vector(){
    this->Destroy();
}

template <typename VecTe>
bool dde::vector<VecTe>::empty() const{
    if (this->stg == nullptr)
        return true;
    return !(this->stg->size > 0);
}

template <typename VecTe>
dde::vector<VecTe>::ulong_t dde::vector<VecTe>::size() const{
    if (this->stg == nullptr)
        return 0;
    return this->stg->size;
}

template <typename VecTe>
dde::vector<VecTe>::ulong_t dde::vector<VecTe>::capacity() const{
    if (this->stg == nullptr)
        return 0;
    return this->stg->capacity;
}

template <typename VecTe>
VecTe &dde::vector<VecTe>::at(const dde::vector<VecTe>::ulong_t &pos){
    if (pos >= this->size()){
        throw std::runtime_error("Termination due to an out-of-bounds error!");
    }
    return *(this->stg->data + pos);
}

template <typename VecTe>
const VecTe &dde::vector<VecTe>::at(const dde::vector<VecTe>::ulong_t &pos) const{
    if (pos >= this->size()){
        throw std::runtime_error("Termination due to an out-of-bounds error!");
    }
    return *(this->stg->data + pos);
}

template <typename VecTe>
dde::vector<VecTe>::data_p_t dde::vector<VecTe>::data(){
    if (this->empty()){
        throw std::runtime_error("The `data()` method cannot be called because `dde::vector` is empty!");
    }
    return this->stg->data;
}

template <typename VecTe>
VecTe &dde::vector<VecTe>::front(){
    if (this->empty()){
        throw std::runtime_error("The `front()` method cannot be called because `dde::vector` is empty!");
    }
    return *this->stg->data;
}

template <typename VecTe>
VecTe &dde::vector<VecTe>::back(){
    if (this->empty()){
        throw std::runtime_error("The `back()` method cannot be called because `dde::vector` is empty!");
    }
    return *(this->stg->data + (this->size() -1));
}

template <typename VecTe>
dde::vector<VecTe>::ulong_t dde::vector<VecTe>::max_size() const{
    return std::numeric_limits<ulong_t>::max();
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::clear(){
    this->DestroyArray(this->stg->data, this->size());
    this->stg->size = 0;
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::resize(const dde::vector<VecTe>::ulong_t &rsize, const VecTe &val){
    if (rsize > this->size()){
        if (rsize >= this->capacity()){
            ulong_t n_cap = this->GrowCapacity(rsize, this->capacity());
            this->reserve(n_cap);
            
            // This is a test. I'm testing the code's execution speed and optimization.
            if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
                if (val != VecTe{}){
                    for (ulong_t i = this->size(); i < rsize; i++){
                        *(this->stg->data +i) = val;
                    }
                }else{
                    std::memset(reinterpret_cast<void*>(this->stg->data + this->size()), 0, sizeof(VecTe) * (rsize - this->size()));
                }
            }else{
                for (ulong_t i = this->size(); i < rsize; i++){
                    std::construct_at<VecTe>(this->stg->data +i, val);
                }
            }
        }
        else{
            if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
                if (val != VecTe{}){
                    for (ulong_t i = this->size(); i < rsize; i++){
                        *(this->stg->data +i) = val;
                    }
                }else{
                    std::memset(reinterpret_cast<void*>(this->stg->data), 0, sizeof(VecTe) * (rsize - this->size()));
                }
            }else{
                for (ulong_t i = this->size(); i < rsize; i++){
                    std::construct_at<VecTe>(this->stg->data +i, val);
                }
            }
        }
    }else{
        if constexpr (!std::is_trivially_destructible_v<VecTe>){
            for (ulong_t i = rsize; i < this->size(); i++){
                std::destroy_at<VecTe>(this->stg->data +i);
            }
        }
        if (rsize != this->size()){
            std::memset(reinterpret_cast<void*>(this->stg->data + rsize), 0, sizeof(VecTe)  * (this->size() - rsize));
        }
    }
    this->stg->size = rsize;
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::reserve(const dde::vector<VecTe>::ulong_t &rcap){
    if (rcap > this->capacity()){
        if (this->capacity() == 0){
            this->stg->data = this->AllocateMemory(this->stg->data, rcap).first;
        }
        else{
            auto r_pair = this->AllocateMemory(this->stg->data, rcap);
            if (r_pair.second == design::copy){
                this->stg->data = r_pair.first;
            }
            else if (r_pair.second == design::created){
                for (ulong_t i = 0; i < this->size(); i++){
                    std::construct_at<VecTe>(r_pair.first + i, std::move(*(this->stg->data + i)));
                }
                this->RemoveArray(&this->stg->data, this->size());
                this->stg->data = r_pair.first;
            }
            else{
                this->RemoveArray(&r_pair.first, 0);
            }
        }
        this->stg->capacity = rcap;
    }
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::push_back(const VecTe &val){
    if (this->size() +1 > this->capacity()){
        this->reserve(this->GrowCapacity(this->size() +1, this->capacity()));
    }
    
    if (this->AppendToTheArray(this->stg->data, this->size(), val)){
        this->stg->size++;
    }
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::push_back(VecTe &&val) noexcept{
    if (this->size() +1 > this->capacity()){
        this->reserve(this->GrowCapacity(this->size() +1, this->capacity()));
    }
    
    if (this->AppendToTheArray(this->stg->data, this->size(), std::move(val))){
        this->stg->size++;
    }
    return *this;
}

template <typename VecTe>
template <typename... Te>
dde::vector<VecTe> &dde::vector<VecTe>::emplace_back(Te &&...t){
    if (this->size() +1 > this->capacity()){
        this->reserve(this->GrowCapacity(this->size() +1, this->capacity()));
    }
    
    std::construct_at<VecTe>(this->stg->data + this->size(), std::forward<Te>(t)...);
    this->stg->size++;
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::pop_back(){
    if (this->size() > 0){
        this->DestroyArray(this->stg->data + this->size() -1, 1);
        this->stg->size--;
    }
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::append_list(const std::initializer_list<VecTe> &list){
    if (list.size() > 0){
        if (this->size() +list.size() > this->capacity()){
            this->reserve(this->GrowCapacity(this->size() +list.size(), this->capacity()));
        }
        
        for (auto it = list.begin(); it != list.end(); it++){
            this->push_back(*it);
        }
    }
    return *this;
}

template <typename VecTe>
template <typename ...Te> requires ((... && std::is_convertible_v<Te, VecTe>) && sizeof...(Te) > 0)
dde::vector<VecTe> &dde::vector<VecTe>::append_va(Te &&...args){
    if (this->size() +sizeof...(args) > this->capacity()){
        this->reserve(this->GrowCapacity(this->size() +sizeof...(args), this->capacity()));
    }
    
    (this->push_back(std::forward<Te>(args)), ...);
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::insert(const dde::vector<VecTe>::ulong_t &pos, const VecTe &val){
    this->insert(pos, {val});
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::insert(const dde::vector<VecTe>::ulong_t &pos, const std::initializer_list<VecTe> &list){
    if (pos < this->size()){
        if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
            ulong_t   cap = this->capacity();
            data_p_t _new = nullptr;
            
            if (this->size() +list.size() > cap){
                cap = this->GrowCapacity(this->size() +list.size(), cap);
            }
            
            _new = this->AllocateMemory(nullptr, cap).first;
            
            if (_new != nullptr){
                ulong_t inc = 0;
                for (auto it = list.begin(); it != list.end(); it++, inc++){
                    *(_new + (pos + inc)) = *it;
                }
                
                if (pos > 0){
                    std::memcpy(reinterpret_cast<void*>(_new), reinterpret_cast<void*>(this->stg->data), sizeof(VecTe) * pos);
                }
                
                if (pos +1 <= this->size()){
                    std::memcpy(reinterpret_cast<void*>(_new + (pos +list.size())), reinterpret_cast<void*>(this->stg->data + pos), sizeof(VecTe) * (this->size() - pos));
                }
                
                this->RemoveArray(&this->stg->data, 0);
                this->stg->data     = _new;
                this->stg->capacity = cap;
                this->stg->size += list.size();
            }
        }else{
            if (this->size() +1 > this->capacity()){
                this->reserve(this->GrowCapacity(this->size() +1, this->capacity()));
            }
            
            // Let's create a few additional objects.
            for (ulong_t i = this->size(); i < this->size() + list.size(); i++){
                std::construct_at<VecTe>(this->stg->data + i, VecTe{});
            }

            for (ulong_t i = this->size() + list.size() -1; i > pos; i--){
                *(this->stg->data + i) = std::move(*(this->stg->data + (i -list.size())));
            }

            for (ulong_t i = pos; i < pos + list.size(); i++){
                *(this->stg->data +i) = *(list.begin() + (i - pos));
            }
            
            this->stg->size += list.size();
        }
    }
    else{
        this->append_list(list);
    }
    return *this;
}

template <typename VecTe>
dde::vector<VecTe> &dde::vector<VecTe>::swap(dde::vector<VecTe> &vec){
    std::swap<Storage*>(this->stg, vec.stg);
    return *this;
}

template <typename VecTe>
VecTe &dde::vector<VecTe>::operator[] (const dde::vector<VecTe>::ulong_t &index){
    return this->at(index);
}

template <typename VecTe>
dde::vector<VecTe>::vector &dde::vector<VecTe>::operator= (const vector<VecTe> &l_value){
    this->clear();
    for (ulong_t i = 0; i < l_value.size(); i++){
        this->push_back(l_value.at(i));
    }
    
    return *this;
}

template <typename VecTe>
dde::vector<VecTe>::vector &dde::vector<VecTe>::operator= (vector<VecTe> &&r_value) noexcept{
    this->clear();
    std::swap(this->stg, r_value.stg); // That's not quite right; I'll fix it a little later.
    
    return *this;
}

template <typename VecTe>
dde::vector<VecTe>::vector &dde::vector<VecTe>::operator= (const std::initializer_list<VecTe> &list){
    this->clear();
    this->append_list(list);
    
    return *this;
}


// Private methods

template <typename VecTe>
dde::vector<VecTe>::Storage *dde::vector<VecTe>::CreateStorage(const dde::vector<VecTe>::data_p_t data, const dde::vector<VecTe>::ulong_t sz, const dde::vector<VecTe>::ulong_t cap){
    Storage *p_stg = static_cast<Storage*>(std::malloc(sizeof(Storage)));
    // This feature is optional since the Storage structure is trivial, but this exception only applies to compilers that support the C20 standard and later.
    std::construct_at<Storage>(p_stg, data, sz, cap); //
    return p_stg;
}

template <typename VecTe>
template <typename Te>
bool dde::vector<VecTe>::AppendToTheArray(dde::vector<VecTe>::data_p_t data, const dde::vector<VecTe>::ulong_t &pos, Te &&val){
    if(data != nullptr){
        if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe> && IMPLICT_LIFETIME_TYPE){
            *(this->stg->data + this->size()) = std::forward<Te>(val);
        }else{
            std::construct_at<VecTe>(data + pos, std::forward<Te>(val));
        }
        return true;
    }
    return false;
}

template <typename VecTe>
std::pair<typename dde::vector<VecTe>::data_p_t, typename dde::vector<VecTe>::design>
dde::vector<VecTe>::AllocateMemory(dde::vector<VecTe>::data_p_t old_data, const dde::vector<VecTe>::ulong_t &cap){
    if (cap > 0){
        void    *data   = nullptr;
        ulong_t  align  = alignof(VecTe);
        ulong_t  sizef  = sizeof(VecTe) * cap;
        
        std::pair<data_p_t, design> r_pair;
        
        if constexpr(std::is_trivially_default_constructible_v<VecTe> && std::is_trivially_copyable_v<VecTe>){
            data = std::realloc(old_data, sizef);
            
            if (data == nullptr){
                r_pair.second = design::null;
            }else{
                r_pair.second = design::copy;
            }
            
        }else{
            if (align <= alignof(std::max_align_t)){
                data = std::malloc(sizef);
            }else{
                if (sizef % align != 0){
                    sizef += align - (sizef % align);
                }
                data = std::aligned_alloc(align, sizef);
            }
            
            if (data == nullptr){
                r_pair.second = design::null;
            }else{
                r_pair.second = design::created;
            }
        }
        
        r_pair.first  = reinterpret_cast<data_p_t>(data);
        
        return r_pair;
    }
    return { nullptr, design::null };
}

template <typename VecTe>
dde::vector<VecTe>::ulong_t dde::vector<VecTe>::GrowCapacity(const dde::vector<VecTe>::ulong_t &size, const dde::vector<VecTe>::ulong_t &cap){
    ulong_t n_cap = (cap == 0 ? 1 : cap);
    while (size >= n_cap) {
        if (n_cap > std::numeric_limits<ulong_t>::max() / 2){
            return std::numeric_limits<ulong_t>::max();
        }
        n_cap *= 2;
    }
    return n_cap;
}

template <typename VecTe>
void dde::vector<VecTe>::RemoveArray(dde::vector<VecTe>::data_p_t *data, const dde::vector<VecTe>::ulong_t &size){
    if (*data != nullptr){
        this->DestroyArray(*data, size);
        std::free(*data);
        *data = nullptr;
    }
}

template <typename VecTe>
void dde::vector<VecTe>::DestroyArray(dde::vector<VecTe>::data_p_t data, const dde::vector<VecTe>::ulong_t &size){
    if (data != nullptr){
        if constexpr (std::is_trivially_destructible_v<VecTe>){
            if (size > 0){
                std::memset(reinterpret_cast<void*>(data), 0, sizeof(VecTe) * size);
            }
        }else{
            for (ulong_t i = 0; i < size; i++){
                std::destroy_at(data + i);
            }
        }
    }
}

template <typename VecTe>
void dde::vector<VecTe>::Destroy(){
    if (this->IsStorage()){
        if (this->IsStorageData()){
            this->RemoveArray(&this->stg->data, this->stg->size);
        }
        std::free(this->stg);
        this->stg = nullptr;
    }
}

template <typename VecTe>
inline bool dde::vector<VecTe>::IsStorage() const{
    return this->stg != nullptr;
}

template <typename VecTe>
inline bool dde::vector<VecTe>::IsStorageData() const{
    return this->stg->data != nullptr;
}
