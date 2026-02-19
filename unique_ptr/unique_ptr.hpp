//
//  unique_ptr.hpp
//  CppKit
//
//  Created by DiCode77.
//

#ifndef unique_ptr_hpp
#define unique_ptr_hpp

namespace dde{

template<typename Te>
class unique_ptr{
    Te *p_data;
public:
    unique_ptr() : p_data(nullptr){}
    unique_ptr(Te *te) : p_data(te){}
    unique_ptr(unique_ptr &&u_ptr) : p_data(u_ptr.p_data){
        u_ptr.p_data = nullptr;
    }
    
    ~unique_ptr(){
        delete this->p_data;
        this->p_data = nullptr;
    }
    
    Te *get() const noexcept{
        return this->p_data;
    }
    
    Te *release() noexcept{
        Te *buff = this->p_data;
        this->p_data = nullptr;
        
        return buff;
    }
    
    void reset(Te *data = nullptr) noexcept{
        if (this->p_data != data){
            delete this->p_data;
            this->p_data = data;
        }
    }
    
    void swap(unique_ptr &u_ptr) noexcept{
        Te *dt = this->p_data;
        this->p_data = u_ptr.p_data;
        u_ptr.p_data = dt;
    }
    
    operator bool() const noexcept{
        return (this->p_data != nullptr) ? true : false;
    }
    
    unique_ptr &operator=(unique_ptr &&u_ptr) noexcept{
        if (this != &u_ptr){
            this->reset(u_ptr.release());
        }
        
        return *this;
    }
    
    Te *operator->() const noexcept{
        return this->p_data;
    }
    
    Te &operator*() const noexcept{
        return *this->p_data;
    }
    
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr &operator=(const unique_ptr&) = delete;
};

};

#endif /* unique_ptr_hpp */
