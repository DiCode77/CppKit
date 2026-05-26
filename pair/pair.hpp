//
//  pair.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef pair_hpp
#define pair_hpp

namespace dde{

template <typename Te_f, typename Te_s>
class pair{
    struct Storage{
        Te_f first;
        Te_s second;
    } *stg;
public:
    pair() : stg(new Storage(Te_f{}, Te_s{})){}
    pair(const Te_f &first, const Te_s &second) : pair(){
        this->stg->first  = first;
        this->stg->second = second;
    }
    
    pair(const pair &p) : pair(){
        this->stg->first  = p.stg->first;
        this->stg->second = p.stg->second;
    }
    
    pair(pair &&p) : pair(){
        Storage *_stg = p.stg;
        p.stg = this->stg;
        this->stg = _stg;
    }
    
    ~pair(){
        delete this->stg;
    }
    
    Te_f &first(){
        return this->stg->first;
    }
    
    Te_s &second(){
        return this->stg->second;
    }
};

}

#endif /* pair_hpp */
