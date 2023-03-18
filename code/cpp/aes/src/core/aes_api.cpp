//
// Created by Luca on 10/02/2023.
//

#include "core/aes_api.hpp"
#include "logger/logger.hpp"

#include "core/aes.hpp"
#include "core/padding/padding.hpp"
#include "core/modes/modes.hpp"
#include "file_manager/file_manager.hpp"
#include "convert/aes_convert.hpp"

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
            return aes::mod::encrypt_CFB(message_with_padding, key, iv.value(), aes); // value_or(std::vector<uint8_t>{}) non funziona? credo
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

// FILE

void encrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::string file_data = aes::fm::FileManager::get_file_data4(input_file_path);
    //std::vector<uint8_t> file_data_vector = aes::cvt::get_vector_from_string(file_data);
    //std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string(key);
    AES_DEBUG("file_data: {}", file_data)
    //std::string file_data = aes::fm::FileManager::get_file_data<std::string>(input_file_path); //TODO: uncomment when fixed

    std::vector<uint8_t> ciphertext = aes::api::encrypt(file_data, key, iv, padding, mode, aes);
    AES_DEBUG("ciphertext: {}", std::string(ciphertext.cbegin(), ciphertext.cend()))
    aes::fm::FileManager::write_file_data(output_file_path, ciphertext); //TODO: std::string(ciphertext.cbegin(), ciphertext.cend())
}

void decrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes)
{
    std::string file_data = aes::fm::FileManager::get_file_data4(input_file_path);
    //std::vector<uint8_t> file_data_vector = aes::cvt::get_vector_from_string(file_data);
    //std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string(key);
    //std::string file_data = aes::fm::FileManager::get_file_data2<std::string>(input_file_path); //TODO: uncomment when fixed
    AES_DEBUG("file_data: {}", file_data)

    std::vector<uint8_t> deciphered_plaintext = aes::api::decrypt(file_data, key, iv, padding, mode, aes);
    AES_DEBUG("deciphered_plaintext: {}", std::string(deciphered_plaintext.cbegin(), deciphered_plaintext.cend()))
    aes::fm::FileManager::write_file_data(output_file_path, deciphered_plaintext); //TODO: std::string(deciphered_plaintext.cbegin(), deciphered_plaintext.cend())
}

} // namespace aes::api
