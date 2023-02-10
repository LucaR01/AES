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

#include "core/padding/padding.hpp"
#include "core/modes/modes.hpp"
#include "core/aes.hpp"

//TODO: rename in aes::api or just aes?
namespace aes::api {

// FUNZIONI DA CHIAMARE

//TODO: encrypt che prende come parametro un vector uint8_t message, vector uint8_t key

//TODO: da chiamare nella console.
std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

//TODO: uncomment
//std::string_view encrypt2(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const pad::Paddings& padding, const mod::Modes& mode, const aes::AES& aes);

std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

} // namespace aes::api

#endif //AES_AES_API_HPP
