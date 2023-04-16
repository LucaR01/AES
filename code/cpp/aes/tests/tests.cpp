//
// Created by Luca on 08/03/2023.
//

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <optional>

#include "gtest/gtest.h"

#include "core/aes.hpp"
#include "core/modes/modes.hpp"
#include "core/padding/padding.hpp"
#include "convert/aes_convert.hpp"
#include "core/aes_api.hpp"

#include "file_manager/file_manager.hpp"

TEST(AES_TESTS, Test_ECB)
{
    static constexpr std::string_view plaintext = "hello everybody "; // 16 characters!
    static constexpr std::string_view key = "secret_key";
    static constexpr aes::AES aes = aes::AES::AES_128;
    std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<std::uint8_t, std::string_view>(plaintext);
    std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<std::uint8_t, std::string_view>(key);

    std::vector<uint8_t> ciphertext = aes::mod::encrypt_ECB(plaintext_vector, key_vector, aes);
    std::vector<uint8_t> deciphered_plaintext = aes::mod::decrypt_ECB(ciphertext, key_vector, aes);

    EXPECT_EQ(plaintext, std::string(deciphered_plaintext.begin(), deciphered_plaintext.end())) << "plaintext: " << plaintext << " deciphered_plaintext: " << aes::cvt::get_string_from_vector<std::string, uint8_t>(deciphered_plaintext);
}

TEST(AES_TESTS, TEST_CBC)
{
    static constexpr std::string_view plaintext = "hello everybody "; // 16 characters (white spaces included)!
    static constexpr std::string_view key = "secret_key";
    static const std::vector<uint8_t> iv = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(plaintext);
    static const std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);

    std::vector<uint8_t> ciphertext = aes::mod::encrypt_CBC(plaintext_vector, key_vector, iv, aes::AES::AES_256);
    std::vector<uint8_t> deciphered_plaintext = aes::mod::decrypt_CBC(ciphertext, key_vector, iv, aes::AES::AES_256);

    EXPECT_EQ(plaintext_vector, deciphered_plaintext);
}

TEST(AES_TESTS, TEST_CFB)
{
    static constexpr std::string_view plaintext = "hello everybody "; // 16 characters (white spaces included)!
    static constexpr std::string_view key = "secret_key";
    static const std::vector<uint8_t> iv = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static const std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(plaintext);
    static const std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);

    std::vector<uint8_t> encrypted_CBC = aes::mod::encrypt_CFB(plaintext_vector, key_vector, iv, aes::AES::AES_256);
    std::vector<uint8_t> decrypted_CBC = aes::mod::decrypt_CFB(encrypted_CBC, key_vector, iv, aes::AES::AES_256);

    EXPECT_EQ(plaintext, std::string_view(aes::cvt::get_string_from_vector<std::string, uint8_t>(decrypted_CBC)));
}

TEST(AES_API_TESTS, TEST_API_MESSAGE)
{
    static std::string_view plaintext = "hello everybody "; // 16 characters (white spaces included)!
    static std::string_view key = "secret_key";
    static const std::vector<uint8_t> iv = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);
    static std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(plaintext);

    std::vector<uint8_t> ciphertext = aes::api::encrypt(plaintext_vector, key_vector, iv, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);
    std::vector<uint8_t> deciphered_plaintext = aes::api::decrypt(ciphertext, key_vector, iv, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    EXPECT_EQ(plaintext, std::string(aes::cvt::get_string_from_vector<std::string, uint8_t>(deciphered_plaintext)));
}

TEST(AES_API_TESTS, TEST_API_FILE)
{
    static const std::string plaintext_file = "plaintext.txt";
    static const std::string encrypted_file = "encrypted.txt";
    static const std::string deciphered_plaintext_file = "deciphered.txt";
    static const std::string plaintext = "hello everybody "; // 16 characters!
    static std::string key = "secret_key";
    static const std::vector<uint8_t> iv = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    std::vector<uint8_t> plaintext_vector = aes::cvt::get_vector_from_string<uint8_t, std::string>(plaintext);
    std::vector<uint8_t> key_vector = aes::cvt::get_vector_from_string<uint8_t, std::string_view>(key);

    aes::fm::FileManager::write_file_data<std::string, std::string>(plaintext_file, plaintext);

    aes::api::encrypt_file(plaintext_file, encrypted_file, key, iv, aes::pad::Paddings::ISO_10126_PADDING, aes::mod::Modes::CBC, aes::AES::AES_256);

    const std::string& encrypted_data = aes::fm::FileManager::get_file_data4(encrypted_file);

    EXPECT_NE(plaintext, encrypted_data);

    aes::api::decrypt_file(encrypted_file, deciphered_plaintext_file, key, iv, aes::pad::Paddings::ISO_10126_PADDING, aes::mod::Modes::CBC, aes::AES::AES_256);

    const std::string& deciphered_plaintext = aes::fm::FileManager::get_file_data4(deciphered_plaintext_file);

    EXPECT_EQ(plaintext, deciphered_plaintext);

    aes::fm::FileManager::delete_file(plaintext_file);
    aes::fm::FileManager::delete_file(encrypted_file);
    aes::fm::FileManager::delete_file(deciphered_plaintext_file);

}