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

//TODO: dove uso constexpr, metterli nell'.hpp; tipo negli enums.

//#define RELEASE_MODE //TODO: uncomment/comment

int main(const int argc, const char** argv)
{

#ifndef RELEASE_MODE
    aes::log::Logger::init();
#endif

    //TODO: posso templetizzare tutti i get_index dei vari enums da poi mettere in un folder generic_templates o generics o templates o generic enums.

    // -----------------------------------------------------------------------------------------------------------------

    //aes::arg::parse_user_arguments(argc, argv); //TODO: uncomment!

    // 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x65, 0x76, 0x65, 0x72, 0x79, 0x62, 0x6F, 0x64, 0x79, 0x20 (hello everybody)

    static const std::string plaintext_file = "plaintext.txt";
    static const std::string encrypted_file = "encrypted.txt";
    static const std::string deciphered_plaintext_file = "deciphered.txt";
    static const std::string plaintext = "hello everybody "; // 16 characters!
    static std::string key = "secret_key";
    static const std::string iv = "1 2 3";
    static const std::optional<std::vector<uint8_t>> iv_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(iv); //std::vector<uint8_t>{0xff, 0xff, 0xff, 0xff, 0xff, 0xff,0xff, 0xff, 0xff, 0xff, 0xff, 0xff,0xff, 0xff, 0xff, 0xff}; //= aes::cvt::get_vector_from_string<uint8_t, std::string_view>(iv);
    std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(plaintext); /*{0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                             0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
                                             0xcc, 0xdd, 0xee, 0xff};*/ //aes::cvt::get_vector_from_string<uint8_t, std::string>(plaintext);
    AES_DEBUG("plaintext_vector: {}", std::string(plaintext_vector.cbegin(), plaintext_vector.cend()))
    std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(key); /*{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                        0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};*/ //aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);

    aes::fm::FileManager::write_file_data<std::string, std::string>(plaintext_file, plaintext);

    aes::api::encrypt_file(plaintext_file, encrypted_file, key, iv_vector, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    const std::string& encrypted_data = aes::fm::FileManager::get_file_data4(encrypted_file);

    AES_DEBUG("encrypted_data: {}", encrypted_data)

    aes::api::decrypt_file(encrypted_file, deciphered_plaintext_file, key, iv_vector, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    const std::string& deciphered_plaintext = aes::fm::FileManager::get_file_data4(deciphered_plaintext_file);

    AES_DEBUG("deciphered_plaintext: {}", deciphered_plaintext)

    aes::fm::FileManager::delete_file(plaintext_file);
    aes::fm::FileManager::delete_file(encrypted_file);
    aes::fm::FileManager::delete_file(deciphered_plaintext_file);

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
