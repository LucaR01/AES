//
// Created by Luca on 10/02/2023.
//

#include "core/aes_api.hpp"
#include "logger/logger.hpp"

#include "core/aes.hpp"
#include "core/padding/padding.hpp"
#include "core/modes/modes.hpp"

namespace aes::api {

// FUNZIONI DA CHIAMARE

std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv,
                             const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::vector<uint8_t> key_vector(key.cbegin(), key.cend());
    std::vector<uint8_t> message_vector(message.cbegin(), message.cend());
    const std::vector<uint8_t>& message_with_padding = aes::pad::add_padding(message_vector, padding);

    AES_DEBUG("unencrypted message_with_padding: {}", std::string(message_with_padding.cbegin(), message_with_padding.cend()))

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

std::vector<uint8_t> encrypt(std::vector<uint8_t>& message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    const std::vector<uint8_t>& message_with_padding = aes::pad::add_padding(message, padding);

    AES_DEBUG("unencrypted message_with_padding: {}", std::string(message_with_padding.cbegin(), message_with_padding.cend()))

    switch(mode) {
        case mod::Modes::ECB:
            return aes::mod::encrypt_ECB(message_with_padding, key, aes);
        case mod::Modes::CBC:
            return aes::mod::encrypt_CBC(message_with_padding, key, iv.value(), aes);
        case mod::Modes::CFB:
            return aes::mod::encrypt_CFB(message_with_padding, key, iv.value(), aes);
        default:
            AES_CRITICAL("Error! Should not be here!")
            std::exit(EXIT_FAILURE);
    }
}

std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv,
                             const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
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

    AES_DEBUG("deciphered plaintext_with_padding: {}", std::string(plaintext_with_padding.cbegin(), plaintext_with_padding.cend()))

    return aes::pad::remove_padding(plaintext_with_padding, padding);
}

std::vector<uint8_t> decrypt(std::vector<uint8_t>& encrypted_message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::vector<uint8_t> plaintext_with_padding{};

    switch(mode) {
        case mod::Modes::ECB:
            plaintext_with_padding = aes::mod::decrypt_ECB(encrypted_message, key, aes);
            break;
        case mod::Modes::CBC:
            plaintext_with_padding =  aes::mod::decrypt_CBC(encrypted_message, key, iv.value(), aes);
            break;
        case mod::Modes::CFB:
            plaintext_with_padding = aes::mod::decrypt_CFB(encrypted_message, key, iv.value(), aes);
            break;
        default:
            AES_CRITICAL("Error! Should not be here!")
            std::exit(EXIT_FAILURE);
    }

    AES_DEBUG("deciphered plaintext_with_padding: {}", std::string(plaintext_with_padding.cbegin(), plaintext_with_padding.cend()))

    return aes::pad::remove_padding(plaintext_with_padding, padding);
}

} // namespace aes::api
