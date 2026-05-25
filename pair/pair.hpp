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
    pair(Te_f first, Te_s second) : pair(){
        this->stg->first  = first;
        this->stg->second = second;
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
