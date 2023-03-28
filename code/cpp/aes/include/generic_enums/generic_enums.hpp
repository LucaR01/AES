//
// Created by Luca on 24/03/2023.
//

#ifndef AES_GENERIC_ENUMS_HPP
#define AES_GENERIC_ENUMS_HPP

#include <type_traits>

namespace aes::en {

template<typename RETURN_TYPE, typename ENUM>
RETURN_TYPE get_index(const ENUM& e)
{
    return static_cast<std::underlying_type_t<ENUM>>(e);
}

//TODO: number of elements in an enum.
//TODO: array with all the elements in the enum.
//TODO: std::map<ENUM, std::string_view>

} // namespace aes::en

#endif //AES_GENERIC_ENUMS_HPP
