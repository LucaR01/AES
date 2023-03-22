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
#include <valarray>
#include <ranges>
#include <variant>

namespace aes::cvt {

[[nodiscard]] static inline std::string get_string_from_vector(const std::vector<uint8_t>& vector) //TODO: remove?
{
    return {vector.cbegin(), vector.cend()};
}

/**
 *
 * @param string : It's the string that has to be converted into a std::vector<uint8_t>
 * @return a std::vector<uint8_t> with the contents of the @param string.
 */
[[nodiscard]] static inline std::vector<uint8_t> get_vector_from_string(const std::string& string) //TODO: remove?
{
    return {string.cbegin(), string.cend()};
}

/**
 * @brief: This function returns a string of @tparam S which has to be either a std::string or a std::string_view with the contents of the std::vector<T>.
 * @tparam S : the type of return of the string.
 * @tparam T : the type of the std::vector @param vector.
 * @param vector : The vector which has to be converted into a string.
 * @return: a string with the contents of the std::vector<T>.
 */
template<typename S, typename T>
[[nodiscard]] static inline S get_string_from_vector(const std::vector<T>& vector) requires std::is_same_v<S, std::string> || std::is_same_v<S, std::string_view>
{
    return {vector.cbegin(), vector.cend()};
}

/**
 * @brief: This function returns a std::vector<T> with the contents of the @param string of type @tparam S.
 * @tparam T : It's the return type of std::vector<T>.
 * @tparam S : It's the type of the @param string which has to be either a std::string or a std::string_view.
 * @param string : It's the string that has to be converted into a std::vector<T>.
 * @return a std::vector<T> with the contents of the @param string.
 */
template<typename T, typename S>
[[nodiscard]] static inline std::vector<T> get_vector_from_string(const S string) requires std::is_same_v<S, std::string> || std::is_same_v<S, std::string_view>
{
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
std::vector<T> from_map_to_vector2(const std::map<K, V>& map, const bool& get_keys) //TODO: remove, non funziona con enum class, ma con enum sì.
{
    std::vector<T> vector(map.size());

    if(get_keys) {
        const auto& key_selector = [&](const auto& pair){ return pair.first; };
        std::transform(map.cbegin(), map.cend(), vector.begin(), key_selector);
    } else {
        const auto& value_selector = [&](const auto& pair){ return pair.second; };
        std::transform(map.cbegin(), map.cend(), vector.begin(), value_selector);
    }

    return vector;
}

//TODO: I would need a Either<std::vector<K>, std::vector<V>> or a variant maybe.
template<typename T, typename K, typename V>
std::vector<T> from_map_to_vector(const std::map<K, V>& map, const bool& get_keys)
{
    std::vector<T> vector;

    for(const auto& m : map) {
        get_keys ? vector.push_back(m.first) : vector.push_back(m.second); // prima era static_cast<const std::_Bit_const_iterator::value_type>(
    }

    return vector;
}

template<typename T, typename K, typename V>
std::vector<T> from_map_to_vector_views(const std::map<K, V>& map, const bool& get_keys) //TODO: auto return type?
{
    if(get_keys) {
        auto keys = std::views::keys(map);
        return static_cast<std::vector<T>>(std::vector(keys.begin(), keys.end())); //TODO: remove static_cast
    }

    auto values = std::views::values(map);

    return static_cast<std::vector<T>>(std::vector{ values.begin(), values.end() }); //TODO: remove static_cast
}

/**
 * @brief You pass a map and a value of the map and you get the corresponding key. It returns an optional, because the value could also be not present in the map.
 * @tparam K : the type of the keys of the std::map
 * @tparam V : the type of the values of the std::map
 * @param map : the map from which we want to retrieve the key.
 * @param value : the value that we're looking for.
 * @return an optional<K> because the value could also be not present in the std::map that we provide.
 */
template<typename K, typename V>
std::optional<K> retrieve_key_from_map(const std::map<K, V>& map, const V& value)
{
    for(const auto& [k, v] : map) {
        //v == value ? return k : std::cout << "nothing"; //TODO: remove
        if(v == value) {
            return std::optional<K>(k);
        }
    }

    return {};
}

/*template<typename K, typename V>
K retrieve_key_from_map(const std::map<K, V>& map, const V& value) //TODO: remove
{
    for(const auto& [k, v] : map) {
        //v == value ? return k : std::cout << "nothing"; //TODO: remove
        if(v == value) {
            return k;
        }
    }

    return;
}*/

/*template<typename K, typename V>
auto retrieve_key_from_map(const std::map<K, V>& map, const V& value) //TODO: remove
{
    for(const auto& [k, v] : map) {
        //v == value ? return k : {}; //TODO: remove?
        if(v == value) {
            return k;
        }
    }

    return; //TODO: return nullptr? return {} doesn't work;
}

template<typename K, typename V>
auto retrieve_key_from_map(const std::map<K, V>& map, const auto& value) //TODO: remove
{
    for(const auto& [k, v] : map) {
        //v == value ? return k : {}; //TODO: remove?
        if(v == value) {
            return k;
        }
    }

    return; //TODO: return nullptr? return {} doesn't work;
}*/

/*template<typename T, typename K, typename V> //TODO: to fix.
std::optional<T> retrieve_key_or_value(const std::map<K, V>& map, const std::variant<K, V>& key_or_value)
{
    for(const auto& [k, v] : map) {
        if(std::holds_alternative<K>) {
            if(k == static_cast<K>(std::get<K>(key_or_value))) {
                return std::optional<T>((T)v);
            }
        } else {
            if(v == std::get<V>(key_or_value)) {
                return std::optional<T>((T)k);
            }
        }
    }

    return {};
}*/

template<typename T, typename K, typename V> //TODO: check if it works
auto retrieve_key_or_value(const std::map<K, V>& map, const std::variant<K, V>& key_or_value)
{
    for(const auto& [k, v] : map) {
        if(std::holds_alternative<K>) {
            if(k == static_cast<K>(std::get<K>(key_or_value))) {
                return std::optional<T>((T)v).value();
            }
        } else {
            if(v == std::get<V>(key_or_value)) {
                return std::optional<T>((T)k).value();
            }
        }
    }

    return; //TODO: nullptr? credo che return; = return nullptr;
}

} // namespace aes::cvt

#endif //AES_AES_CONVERT_HPP
