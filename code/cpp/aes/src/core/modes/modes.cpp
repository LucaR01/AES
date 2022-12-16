//
// Created by Luca on 14/12/2022.
//

#include <iostream>
#include <type_traits>

#include "core/modes/modes.hpp"

namespace aes::mod {

unsigned short get_mode_index(const Modes& mode)
{
    return static_cast<std::underlying_type_t<Modes>>(mode);
}

}
