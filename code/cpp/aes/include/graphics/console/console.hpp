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

void show_console(const std::pair<aes::AES, arg::Arguments>& aes_argument = std::make_pair(aes::def::DEFAULT_AES, arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::mod::Modes, arg::Arguments>& mode_argument = std::make_pair(aes::def::DEFAULT_MODE, arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::pad::Paddings, arg::Arguments>& padding_argument = std::make_pair(aes::def::DEFAULT_PADDING, arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, arg::Arguments>& message_argument = std::make_pair("", arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<std::string, aes::arg::Arguments>& key_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, aes::arg::Arguments>& iv_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, aes::arg::Arguments>& input_file_path_argument = std::make_pair("", aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT),
                  const std::pair<std::string, arg::Arguments>& output_file_path_argument = std::make_pair(std::string(aes::def::DEFAULT_OUTPUT_FILE_PATH), arg::Arguments::NOT_USER_PASSED_ARGUMENT), const std::pair<aes::ops::Operations, arg::Arguments>& operation_argument = std::make_pair(aes::def::DEFAULT_OPERATION, arg::Arguments::NOT_USER_PASSED_ARGUMENT)); //TODO: rename in show()?

//void get_user_input();

// ENCRYPTION & DECRYPTION

//TODO: rename in qualcosa di più carino.
//TODO. "OLD"
//void operation_encryption_decryption(const ops::Operations& operation); //TODO: request_file_or_message_operation() o request_operation_file_or_message o request_encryption_object o request_encryption_operation

//TODO: "NEW"
void encrypt_decrypt_message_or_file(const aes::ops::EncryptionOperations& encryption_operation, const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding,
                             const std::vector<uint8_t>& iv, const std::string& input_file_path, const std::string& output_file_path); //TODO: rename in encrypt_decrypt_message_or_file();

//TODO. "OLD"
//void show_encrypt_decrypt_message(const ops::Operations& operation);

//TODO: optional iv?
//TODO: "NEW"
void encrypt_decrypt_message(const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv);

//TODO. "OLD"
//void show_encrypt_decrypt_file(const ops::Operations& operation);

//TODO: "NEW"
void encrypt_decrypt_file(const aes::ops::Operations& operation, std::string input_file_data, const std::string& output_file_path, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv);


// REQUEST

aes::ops::Operations request_operation(); //TODO: o magari rinominarla get_operation();

aes::ops::EncryptionOperations request_file_or_message_operation();

std::string request_message();

std::string request_input_file();

std::string request_output_file();

aes::pad::Paddings request_padding(); //TODO: rename in get_padding() o altro?

std::string request_key();

aes::mod::Modes request_mode();

std::vector<uint8_t> request_iv(const aes::mod::Modes& mode); //TODO: rename in selected_mode

aes::AES request_aes_type(); //TODO: rename in request_aes_size() oppure request_aes?

//TODO: std::string request_IV();

} // namespace aes::con

#endif //AES_CONSOLE_H
