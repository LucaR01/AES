//
// Created by Luca on 10/02/2023.
//

#include "core/aes_api.hpp"
#include "logger/logger.hpp"

namespace aes::api {

// FUNZIONI DA CHIAMARE

std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv,
                             const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::vector<uint8_t> key_vector(key.cbegin(), key.cend());
    std::vector<uint8_t> message_vector(message.cbegin(), message.cend());
    const std::vector<uint8_t>& message_with_padding = aes::pad::add_padding(message_vector, padding);

    AES_DEBUG("message_with_padding: {}", std::string(message_with_padding.cbegin(), message_with_padding.cend()))

/*#ifndef RELEASE_MODE //TODO: remove
    //std::string_view sv(message_with_padding, message_with_padding.size());
    for(const auto& mp : message_with_padding) {
        std::cout << mp;
    }

    std::cout << std::flush;
#endif*/

    switch(mode) {
        case mod::Modes::ECB:
            return aes::mod::encrypt_ECB(message_with_padding, key_vector, aes);
        case mod::Modes::CBC:
            return aes::mod::encrypt_CBC(message_with_padding, key_vector, iv.value(), aes);
        case mod::Modes::CFB:
            return aes::mod::encrypt_CFB(message_with_padding, key_vector, iv.value(), aes);
        default:
            AES_CRITICAL("Error! Should not be here!")
            std::exit(EXIT_FAILURE);
    }
}

std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::vector<uint8_t> key_vector(key.cbegin(), key.cend());
    std::vector<uint8_t> encrypted_message_vector(encrypted_message.cbegin(), encrypted_message.cend());

    std::vector<uint8_t> plaintext_with_padding{};

    switch(mode) {
        case mod::Modes::ECB:
            plaintext_with_padding = aes::mod::decrypt_ECB(encrypted_message_vector, key_vector, aes);
            break;
        case mod::Modes::CBC:
            plaintext_with_padding =  aes::mod::decrypt_CBC(encrypted_message_vector, key_vector, iv.value(), aes);
            break;
        case mod::Modes::CFB:
            plaintext_with_padding = aes::mod::decrypt_CFB(encrypted_message_vector, key_vector, iv.value(), aes);
            break;
        default:
            AES_CRITICAL("Error! Should not be here!")
            std::exit(EXIT_FAILURE);
    }

    AES_DEBUG("plaintext_with_padding: {}", std::string(plaintext_with_padding.cbegin(), plaintext_with_padding.cend()))

    return aes::pad::remove_padding(plaintext_with_padding, padding);
}

} // namespace aes::api
