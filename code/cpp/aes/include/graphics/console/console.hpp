//
// Created by Luca on 09/12/2022.
//

#ifndef AES_CONSOLE_H
#define AES_CONSOLE_H

#include <string_view>

#include "core/padding/padding.hpp"
#include "core/modes/modes.hpp"
#include "core/operations/operations.hpp"

namespace aes::con {

//TODO: usare <format> quando possibile.

//TODO: Singleton Class Console, con private tutto tranne show_console();
//TODO: volendo potrei combinare show_encrypt_message() e show_decrypt_message() passando un enum Operations a una funzione
//TODO: show_encrypt_decrypt_message();

//TODO: static constexpr?

void show_console();

void get_user_input();

// ENCRYPTION

void operation_encryption();

void show_encrypt_message();

void show_encrypt_file();

// DECRYPTION

void operation_decryption();

void show_decrypt_message();

void show_decrypt_file();

// ENCRYPTION & DECRYPTION

void show_encrypt_decrypt_message(const ops::Operations& operation); //TODO: remove?; rename in encrypt_decrypt_message?

void show_encrypt_decrypt_file(const ops::Operations& operation); //TODO: remove?; rename in encrypt_decrypt_file?

// REQUEST

aes::ops::Operations request_operation(); //TODO: o magari rinominarla get_operation();

std::string request_message();

std::string request_input_file();

std::string request_output_file();

aes::pad::Paddings request_padding(); //TODO: rename in get_padding() o get_input_padding() o altro?

std::string request_key();

//std::string request_key_from_file();

aes::mod::Modes request_mode();

}

#endif //AES_CONSOLE_H
