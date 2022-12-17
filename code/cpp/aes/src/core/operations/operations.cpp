//
// Created by Luca on 14/12/2022.
//

#include "core/operations/operations.hpp"

namespace aes::ops {

unsigned short get_operation_index(const std::variant<Operations, EncryptionOperations>& op)
{
    if(std::holds_alternative<Operations>(op)) {
        return static_cast<std::underlying_type_t<Operations>>(std::get<Operations>(op));
    } else {
        return static_cast<std::underlying_type_t<Operations>>(std::get<EncryptionOperations>(op));
    }
}

}
