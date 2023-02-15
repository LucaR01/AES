//
// Created by Luca on 10/02/2023.
//

#ifndef AES_AES_API_HPP
#define AES_AES_API_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <string_view>
#include <optional>

// Forward Declaration to avoid Circular Dependency of Header files.
// In realtà non c'era bisogno, ma velocizza il compile time.
namespace aes::pad {
    enum class Paddings;
}

namespace aes::mod {
    enum class Modes;
}

namespace aes {
    enum class AES;
}

//TODO: rename in aes::api or just aes?
namespace aes::api {

// FUNZIONI DA CHIAMARE

//TODO: encrypt che prende come parametro un vector uint8_t message, vector uint8_t key

//TODO: usare string_view?
//TODO: std::optional = std::nullopt_t as default value
std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

//TODO: creare una encrypt e una decrypt che prende un std::vector<uint8_t> come message e come key?

//TODO: std::optional = std::nullopt_t as default value
std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

} // namespace aes::api

#endif //AES_AES_API_HPP
