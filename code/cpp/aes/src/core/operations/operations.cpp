//
// Created by Luca on 14/12/2022.
//

#include "core/operations/operations.hpp"

namespace aes::ops {

unsigned short get_operation_index(Operations operation)
{
    return static_cast<std::underlying_type_t<Operations>>(operation);
}

}
