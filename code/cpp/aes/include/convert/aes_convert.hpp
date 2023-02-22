//
// Created by Luca on 15/02/2023.
//

#ifndef AES_AES_CONVERT_HPP
#define AES_AES_CONVERT_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

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

/**
 * @version: AES v1.0.0
 * @brief: This function permits the developer to retrieve a vector composed of keys or values of a std::map.
 * Usage: aes::cvt::from_map_to_vector<std::string>(map, true) to get the keys of the map in a vector; false to get values.
 *
 * @tparam T
 * @tparam K : template parameter type of the keys of the std::map.
 * @tparam V : template parameter type of the values of the std::map;
 * @param map : an std::map
 * @param get_keys : whether you want to retrieve the keys or the values of the @param map.
 * @return : an std::vector of type @tparam T.
 */
template<typename T, typename K, typename V>
std::vector<T> from_map_to_vector(const std::map<K, V>& map, const bool& get_keys)
{
    std::vector<T> vector(map.size());

    if(get_keys) {
        //std::vector<T> keys(map.size()); //TODO: remove
        const auto& key_selector = [&](const auto& pair){ return pair.first; };
        std::transform(map.cbegin(), map.cend(), vector.begin(), key_selector);
        //return vector; //TODO: remove
    } else {
        const auto& value_selector = [&](const auto& pair){ return pair.second; };
        std::transform(map.cbegin(), map.cend(), vector.begin(), value_selector);
    }

    //std::vector<T> values(map.size()); //TODO: remove

    return vector;
}

} // namespace aes::cvt

#endif //AES_AES_CONVERT_HPP
