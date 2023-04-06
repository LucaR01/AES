#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <memory>
#include <string_view>


#include "logger/logger.hpp"
#include "core/aes_api.hpp"
#include "graphics/console/console.hpp"
#include "file_manager/file_manager.hpp" //TODO: remove?
#include "arguments_parser/arguments_parser.hpp"
#include "convert/aes_convert.hpp"

#define RELEASE_MODE //TODO: uncomment/comment when in release mode.

int main(const int argc, const char** argv)
{

#ifndef RELEASE_MODE
    aes::log::Logger::init();
#endif

    aes::arg::parse_user_arguments(argc, argv); //TODO: uncomment!

    // -----------------------------------------------------------------------------------------------------------------

    //TODO: remove
    /*static const std::string plaintext_file = "plaintext.txt";
    static const std::string encrypted_file = "encrypted.txt";
    static const std::string deciphered_plaintext_file = "deciphered.txt";
    static const std::string plaintext = "hello everybody "; // 16 characters!
    static std::string key = "secret_key";
    static const std::string iv = "1 2 3";
    static const std::optional<std::vector<uint8_t>> iv_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(iv);
    std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(plaintext);
    AES_DEBUG("plaintext_vector: {}", std::string(plaintext_vector.cbegin(), plaintext_vector.cend()))
    std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(key);

    aes::fm::FileManager::write_file_data<std::string, std::string>(plaintext_file, plaintext);

    aes::api::encrypt_file(plaintext_file, encrypted_file, key, iv_vector, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    const std::string& encrypted_data = aes::fm::FileManager::get_file_data4(encrypted_file);

    AES_DEBUG("encrypted_data: {}", encrypted_data)

    aes::api::decrypt_file(encrypted_file, deciphered_plaintext_file, key, iv_vector, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    const std::string& deciphered_plaintext = aes::fm::FileManager::get_file_data4(deciphered_plaintext_file);

    AES_DEBUG("deciphered_plaintext: {}", deciphered_plaintext)

    aes::fm::FileManager::delete_file(plaintext_file);
    aes::fm::FileManager::delete_file(encrypted_file);
    aes::fm::FileManager::delete_file(deciphered_plaintext_file);*/

    // -----------------------------------------------------------------------------------------------------------------

    /*static constexpr std::string_view plaintext = "hello everybody "; // 16 characters (white spaces included)!
    static constexpr std::string_view key = "secret_key";
    static constexpr std::string_view iv = "1 2 3";
    static const std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(plaintext);
    static const std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);
    static const std::vector<uint8_t> iv_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(iv);

    //TODO: uncomment
    std::vector<uint8_t> ciphertext = aes::mod::encrypt_CBC(plaintext_vector, key_vector, iv_vector, aes::AES::AES_256);
    std::vector<uint8_t> deciphered_plaintext = aes::mod::decrypt_CBC(ciphertext, key_vector, iv_vector, aes::AES::AES_256);

    std::cout << "plaintext: " << plaintext << std::endl << " ciphertext: " << aes::cvt::get_string_from_vector<std::string, uint8_t>(ciphertext) << " deciphered_plaintext: " << aes::cvt::get_string_from_vector<std::string, uint8_t>(deciphered_plaintext);*/

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
