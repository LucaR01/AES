//
// Created by Luca on 09/03/2023.
//

#ifndef AES_DEFAULTS_HPP
#define AES_DEFAULTS_HPP

#include <string_view>

#include "core/aes.hpp"
#include "core/modes/modes.hpp"
#include "core/padding/padding.hpp"
#include "core/operations/operations.hpp"

namespace aes::def {

static constexpr aes::AES DEFAULT_AES = aes::AES::AES_128; //TODO: 256?
static constexpr aes::mod::Modes DEFAULT_MODE = aes::mod::Modes::ECB; //TODO: CBC/CFB?
static constexpr aes::pad::Paddings DEFAULT_PADDING = aes::pad::Paddings::NO_PADDING; //TODO: ISO_10126_PADDING?
static constexpr aes::ops::Operations DEFAULT_OPERATION = aes::ops::Operations::ENCRYPT;
static constexpr aes::ops::EncryptionOperations DEFAULT_ENCRYPTION_OPERATION = aes::ops::EncryptionOperations::MESSAGE;
static constexpr std::string_view DEFAULT_OUTPUT_FILE_PATH = "output.txt";
static constexpr std::string_view DEFAULT_GRAPHICS_VIEW = "--console"; //TODO: remove?
static constexpr std::string_view EMPTY_STRING; // = "";

} // namespace aes::def

#endif //AES_DEFAULTS_HPP
