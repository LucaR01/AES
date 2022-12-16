//
// Created by Luca on 14/12/2022.
//

#include "core/operations/operations.hpp"

namespace aes::ops {

//template<class... Ts> struct overload : Ts... { using Ts::operator()...; }; //TODO: remove
//template<class... Ts> overload(Ts...) -> overload<Ts...>;

unsigned short get_operation_index(const std::variant<Operations, EncryptionOperations>& op)
{
    //return static_cast<std::underlying_type_t<Operations>>(op); //TODO: remove
    /*std::visit(overload{
        [](Operations& ) { return static_cast<std::underlying_type_t<Operations>>(op); },
        [](EncryptionOperations& ) { return static_cast<std::underlying_type_t<Operations>>(op); }
    });*/
    if(std::holds_alternative<Operations>(op)) { //std::get<Operations>(op) //TODO: oppure std::variant::index
        return static_cast<std::underlying_type_t<Operations>>(std::get<Operations>(op));
    } else {
        return static_cast<std::underlying_type_t<Operations>>(std::get<EncryptionOperations>(op));
    }
}

}
