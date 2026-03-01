//
//  weak_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef weak_ptr_hpp
#define weak_ptr_hpp

#include "shared_ptr.hpp"

namespace dde{

template <typename Te>
class weak_ptr{
    dde::shared_ptr<Te>::CONTROL_BK *con_bk;
public:
    weak_ptr() : con_bk(nullptr){}
    weak_ptr(const dde::shared_ptr<Te> &sh_ptr) : weak_ptr(){
        if (!sh_ptr.empty()){
            this->con_bk = sh_ptr.con_bk;
            this->con_bk->weak += 1;
        }
    }
    
    ~weak_ptr(){
        if (this->con_bk->weak > 0)
            this->con_bk->weak -= 1;
        
        if (!this->con_bk->strong && !this->con_bk->weak){
            delete this->con_bk;
            this->con_bk = nullptr;
        }
    }
};

}
#endif /* weak_ptr_hpp */
