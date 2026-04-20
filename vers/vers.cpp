#include "vers.hpp"

dde::vers::~vers(){
    this->DestroyEvrything();
}

dde::vers::vers() : stg(this->CreateStorage()){}
