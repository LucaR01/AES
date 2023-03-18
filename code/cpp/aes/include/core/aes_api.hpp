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

namespace aes::api {

// FUNZIONI DA CHIAMARE

// MESSAGE

//TODO: usare string_view?
//TODO: std::optional = std::nullopt_t as default value
std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

std::vector<uint8_t> encrypt(std::vector<uint8_t>& message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

//TODO: std::optional = std::nullopt_t as default value
std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

std::vector<uint8_t> decrypt(std::vector<uint8_t>& encrypted_message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

// FILE
//TODO: const fm::FileModes& file_mode = FileModes::APPEND?
void encrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

void decrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

} // namespace aes::api

#endif //AES_AES_API_HPP
