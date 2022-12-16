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

void show_console();

void get_user_input();

aes::ops::Operations request_operation(); //TODO: o magari rinominarla get_operation();

void operation_encryption();

void show_encrypt_message();

void show_encrypt_file();

void operation_decryption();

void show_decrypt_message();

void show_decrypt_file();

aes::pad::Paddings request_padding(); //TODO: rename in get_padding() o get_input_padding() o altro?

std::string_view request_key();

aes::mod::Modes request_mode();

}

#endif //AES_CONSOLE_H
