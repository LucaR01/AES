//
// Created by Luca on 15/02/2023.
//

#ifndef AES_AES_CONVERT_HPP
#define AES_AES_CONVERT_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace aes::cvt {

[[nodiscard]] static inline std::string get_string_from_vector(const std::vector<uint8_t>& vector)
{
    return {vector.cbegin(), vector.cend()};
}

[[nodiscard]] static inline std::vector<uint8_t> get_vector_from_string(const std::string& string)
{
    //return std::vector<uint8_t>(string.cbegin(), string.cend()); //TODO: remove
    return {string.cbegin(), string.cend()};
}

} // namespace aes::cvt

#endif //AES_AES_CONVERT_HPP
