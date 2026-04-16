#include "vers.hpp"

dde::vers::vers() : stg(static_cast<dde::vers::Storage*>(std::malloc(sizeof(Storage)))){}
