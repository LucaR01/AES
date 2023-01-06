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

void operation_encryption(); //TODO: remove?

void show_encrypt_message(); //TODO: remove?

void show_encrypt_file(); //TODO: remove?

// DECRYPTION

void operation_decryption(); //TODO: remove?

void show_decrypt_message(); //TODO: remove?

void show_decrypt_file(); //TODO: remove?

// ENCRYPTION & DECRYPTION

//TODO: rename in qualcosa di più carino.
void operation_encryption_decryption(const ops::Operations& operation);

void show_encrypt_decrypt_message(const ops::Operations& operation);

void show_encrypt_decrypt_file(const ops::Operations& operation);

// REQUEST

aes::ops::Operations request_operation(); //TODO: o magari rinominarla get_operation();

std::string request_message();

std::string request_input_file();

std::string request_output_file();

aes::pad::Paddings request_padding(); //TODO: rename in get_padding() o altro?

std::string request_key();

aes::mod::Modes request_mode();

}

#endif //AES_CONSOLE_H
