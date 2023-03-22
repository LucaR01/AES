//
// Created by Luca on 09/12/2022.
//

#ifndef AES_CONSOLE_H
#define AES_CONSOLE_H

#include <string>
#include <string_view>
#include <cstdint>
#include <vector>
#include <utility>

#include "core/aes.hpp"
#include "core/padding/padding.hpp"
#include "core/modes/modes.hpp"
#include "core/operations/operations.hpp"
#include "defaults/defaults.hpp"

#include "arguments_parser/arguments_parser.hpp"

namespace aes::con {

//TODO: usare <format> quando possibile.

//TODO: Singleton Class Console, con private tutto tranne show_console();
//TODO: volendo potrei combinare show_encrypt_message() e show_decrypt_message() passando un enum Operations a una funzione
//TODO: show_encrypt_decrypt_message();

//TODO: static constexpr?

/**
 * @brief: This function shows the console to the user. The parameters are taken from the arguments passed by the user otherwise the default value is used.
 * @param aes_argument : The first element of the pair is @enum AES, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param mode_argument : The first element of the pair is @enum Modes, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param padding_argument : The first element of the pair is @enum Paddings, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param message_argument : The first element of the pair is a std::string message, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param key_argument : The first element of the pair is a std::string key, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param iv_argument : The first element of the pair is a std::string iv, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param input_file_path_argument : The first element of the pair is a string with the input file, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param output_file_path_argument : The first element of the pair is a string with the output file, the second @enum Arguments indicates whether is a user passed argument or not.
 * @param operation_argument : The first element of the pair is @enum Operations, the second @enum Arguments indicates whether is a user passed argument or not.
 */
void show_console(const std::pair<aes::AES, arg::Arguments>& aes_argument = std::make_pair(aes::def::DEFAULT_AES, arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::mod::Modes, arg::Arguments>& mode_argument = std::make_pair(aes::def::DEFAULT_MODE, arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::pad::Paddings, arg::Arguments>& padding_argument = std::make_pair(aes::def::DEFAULT_PADDING, arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, arg::Arguments>& message_argument = std::make_pair("", arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<std::string, aes::arg::Arguments>& key_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, aes::arg::Arguments>& iv_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, aes::arg::Arguments>& input_file_path_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, arg::Arguments>& output_file_path_argument = std::make_pair(std::string(aes::def::DEFAULT_OUTPUT_FILE_PATH), arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::ops::Operations, arg::Arguments>& operation_argument = std::make_pair(aes::def::DEFAULT_OPERATION, arg::Arguments::NOT_USER_PASSED_ARGUMENT)); //TODO: rename in show()?

//void get_user_input();

// ENCRYPTION & DECRYPTION

/**
 * @brief This function calls a function to encrypt a message or a file depending on the value of the @p encryption_operation.
 * @param encryption_operation: It's a @enum EncryptionOperations which determines if you want to encrypt/decrypt a file or a message.
 * @param operation : @enum Operation indicates whether we want to encrypt or decrypt.
 * @param message : the message that we want to encrypt/decrypt.
 * @param key : The key that we want to use for the encryption/decryption.
 * @param aes : The type of @enum AES: 128, 192 or 256.
 * @param mode : @enum Modes indicates which mode we want to use for encryption/decryption.
 * @param padding : @enum Paddings indicates which padding we want to use.
 * @param iv : the initialization vector.
 * @param input_file_path : the file from which we want to retrieve the plaintext or encrypted text.
 * @param output_file_path : the file where we want to store the encrypted text or deciphered plaintext.
 */
void encrypt_decrypt_message_or_file(const aes::ops::EncryptionOperations& encryption_operation, const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding,
                             const std::vector<uint8_t>& iv, const std::string& input_file_path, const std::string& output_file_path); //TODO: rename in encrypt_decrypt_message_or_file();

//TODO: optional iv?
/**
 * @brief This function encrypts or decrypts a message.
 * @param operation: indicates whether we want to encrypt or decrypt.
 * @param message: the message that we want to encrypt/decrypt.
 * @param key: The key that we want to use for the encryption/decryption.
 * @param aes: The type of AES: 128, 192 or 256.
 * @param mode: indicates which mode we want to use for encryption/decryption.
 * @param padding: indicates which padding we want to use.
 * @param iv: the initialization vector.
 */
void encrypt_decrypt_message(const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv);

/**
 * @brief This function retrieve the data from the input_file_data and store the encrypted or decrypted data in the output_file_path.
 * @param operation indicates whether we want to encrypt or decrypt.
 * @param input_file_data: it's the data of the input file. (not the path of the input file!)
 * @param output_file_path: it's the file where we want to store the encrypted text or deciphered plaintext.
 * @param key: The key that we want to use for the encryption/decryption.
 * @param aes: The type of AES: 128, 192 or 256.
 * @param mode: indicates which mode we want to use for encryption/decryption.
 * @param padding: indicates which padding we want to use.
 * @param iv: the initialization vector.
 */
void encrypt_decrypt_file(const aes::ops::Operations& operation, std::string input_file_data, const std::string& output_file_path, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv);

// REQUEST

/**
 * @brief This function asks the user which operation does he/she want to run: Encryption or Decryption.
 * @return an @enum Operations
 */
aes::ops::Operations request_operation(); //TODO: o magari rinominarla get_operation();

/**
 * @brief This function requests the user whether he/she wants to encrypt/decrypt a file or a message.
 * @return @enum EncryptionOperations MESSAGE, FILE.
 */
aes::ops::EncryptionOperations request_file_or_message_operation();

/**
 * @brief This function requests the user the message to encrypt/decrypt.
 * @return a string of the message.
 */
std::string request_message();

/**
 * @brief This function requests the user the input file from which to retrieve the data.
 * @return a string of the input file.
 */
std::string request_input_file();

/**
 * @brief This function requests the user the output file where he/she wants to store the encrypted/decrypted data.
 * @return a string of the output file path.
 */
std::string request_output_file();

/**
 * @brief This function requests the user the padding that has to be used.
 * @return @enum Paddings.
 */
aes::pad::Paddings request_padding(); //TODO: rename in get_padding() o altro?

/**
 * @brief This function requests the user the key to use for encryption/decryption.
 * @return a string of the key.
 */
std::string request_key();

/**
 * @brief This function requests the user the mode that has to be used.
 * @return @enum Modes.
 */
aes::mod::Modes request_mode();

/**
 *
 * @param mode : the mode to be used, because if it's ECB then the IV is not required.
 * @return a std::vector<uint8_t> with the initialization vector.
 */
std::vector<uint8_t> request_iv(const aes::mod::Modes& mode); //TODO: rename in selected_mode

/**
 * @brief This function requests the user the AES type that has to be used.
 * @return @enum AES.
 */
aes::AES request_aes_type(); //TODO: rename in request_aes_size() oppure request_aes?

} // namespace aes::con

#endif //AES_CONSOLE_H
