//
// Created by Luca on 14/12/2022.
//

#include <type_traits>

#include "core/padding/padding.hpp"

namespace aes::pad {

unsigned short get_padding_index(const Paddings& padding)
{
    return static_cast<std::underlying_type_t<Paddings>>(padding);
}

}
