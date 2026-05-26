//
//  pair.inl
//  CppKit
//
//  Created by DiCode77.
//
// This file contains the implementation of the template methods for the dde::pair class.

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s>::pair() : stg(new Storage(Te_f{}, Te_s{})){}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s>::pair(const Te_f &first, const Te_s &second) : dde::pair<Te_f, Te_s>::pair(){
    this->stg->first  = first;
    this->stg->second = second;
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s>::pair(const dde::pair<Te_f, Te_s> &p) : dde::pair<Te_f, Te_s>::pair(){
    this->stg->first  = p.stg->first;
    this->stg->second = p.stg->second;
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s>::pair(dde::pair<Te_f, Te_s> &&p) noexcept : dde::pair<Te_f, Te_s>::pair(){
    this->swap(p);
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s>::~pair(){
    delete this->stg;
}

template <typename Te_f, typename Te_s>
Te_f &dde::pair<Te_f, Te_s>::first(){
    return this->stg->first;
}

template <typename Te_f, typename Te_s>
Te_s &dde::pair<Te_f, Te_s>::second(){
    return this->stg->second;
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s> &dde::pair<Te_f, Te_s>::swap(dde::pair<Te_f, Te_s> &sw){
    Storage *_stg = sw.stg;
    sw.stg    = this->stg;
    this->stg = _stg;
    return *this;
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s> &dde::pair<Te_f, Te_s>::operator= (const dde::pair<Te_f, Te_s> &p){
    this->stg->first  = p.stg->first;
    this->stg->second = p.stg->second;
    
    return *this;;
}

template <typename Te_f, typename Te_s>
dde::pair<Te_f, Te_s> &dde::pair<Te_f, Te_s>::operator= (dde::pair<Te_f, Te_s> &&p) noexcept{
    delete this->stg;
    this->stg = p.stg;
    p.stg = new Storage(Te_f{}, Te_s{});
    
    return *this;
}
