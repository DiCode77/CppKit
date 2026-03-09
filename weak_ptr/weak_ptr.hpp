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
    
    weak_ptr &operator= (const dde::shared_ptr<Te> &sh_obj){
        if (!this->empty()){
            this->con_bk->weak -= 1;
        }
        
        this->con_bk = sh_obj.con_bk;
        this->con_bk->weak += 1;
        return *this;
    }
    
    bool empty() const{
        return this->con_bk == nullptr ? true : false;
    }
    
    dde::shared_ptr<Te> lock() const{
        return dde::shared_ptr<Te>(this->con_bk);
    }
};

}
#endif /* weak_ptr_hpp */
