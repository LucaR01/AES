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

/**
 * @brief: This function encrypts the inputted message and returns it encrypted.
 * @param message : The plaintext that has to be encrypted.
 * @param key : The key to be used for encryption.
 * @param iv : the initialization vector.
 * @param padding : The @enum Paddings to be used for encryption.
 * @param mode : The @enum Modes to be used for encryption.
 * @param aes : The @enum AES to select the type of AES to be used: 128, 192 or 256.
 * @return : the encrypted message as a std::vector<uint8_t>. (uint8_t is an unsigned char)
 */
std::vector<uint8_t> encrypt(std::vector<uint8_t>& message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

//TODO: std::optional = std::nullopt_t as default value
std::vector<uint8_t> decrypt(std::string& encrypted_message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

/**
 * @brief: This function decrypts the inputted encrypted message and returns it decrypted.
 * @param encrypted_message : The encrypted message that has to be decrypted.
 * @param key : The key to be used for decryption that has been used in the encryption.
 * @param iv : the initialization vector.
 * @param padding : The @enum Paddings to be used for decryption that has been used in the encryption.
 * @param mode : The @enum Modes to be used for decryption that has been used in the encryption.
 * @param aes : The @enum AES to select the type of AES to be used: 128, 192 or 256.
 * @return : the decrypted plaintext message as a std::vector<uint8_t>. (uint8_t is an unsigned char)
 */
std::vector<uint8_t> decrypt(std::vector<uint8_t>& encrypted_message, std::vector<uint8_t>& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

// FILE
//TODO: const fm::FileModes& file_mode = FileModes::APPEND?
/**
 * @brief: This function encrypts the contents of a file and store them encrypted in another file or in the same input file.
 * @param input_file_path : The file with the plaintext to be encrypted.
 * @param output_file_path : The file where we want to store the encrypted text.
 * @param key : The key to be used for the encryption.
 * @param iv : the initialization vector.
 * @param padding : The @enum Paddings to be used for the encryption.
 * @param mode : The @enum Modes to be used for encryption.
 * @param aes : The @enum AES to select the type of AES to be used: 128, 192 or 256.
 */
void encrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

/**
 * @brief: This function decrypts the contents of a file and store them decrypted in another file or in the same input file.
 * @param input_file_path : The file with the encrypted text to be decrypted.
 * @param output_file_path : The file where we want to store the decrypted plaintext.
 * @param key : The key to be used for the decryption that has been used for encryption too.
 * @param iv : the initialization vector.
 * @param padding : The @enum Paddings to be used for the decryption.
 * @param mode : The @enum Modes to be used for decryption.
 * @param aes : The @enum AES to select the type of AES to be used: 128, 192 or 256.
 */
void decrypt_file(const std::string& input_file_path, const std::string& output_file_path, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const aes::pad::Paddings& padding, const aes::mod::Modes& mode, const aes::AES& aes);

} // namespace aes::api

#endif //AES_AES_API_HPP
