//
// Created by Luca on 09/12/2022.
//

#ifndef AES_MODES_H
#define AES_MODES_H

#include <map>
#include <string_view>
#include <array>
#include <cstdint>
#include <vector>

#include "core/aes.hpp"

namespace aes::mod {

enum class Modes {
    ECB = 1,
    CBC,
    CTR
};

static constexpr unsigned short NUM_OF_MODES = 3;

static constexpr std::array<Modes, NUM_OF_MODES> all = { Modes::ECB, Modes::CBC, Modes::CTR };

//TODO: static constexpr?
//TODO: ritornare un byte? uint8_t? char?
//TODO: [[nodiscard]]
unsigned short get_mode_index(const Modes& mode);

static const std::map<Modes, std::string_view> modes_names = {
        {Modes::ECB, "ECB"},
        {Modes::CBC, "CBC"},
        {Modes::CTR, "CTR"}
};

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const uint16_t& number_of_rounds );

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes );

std::vector<uint8_t> decrypt_ECB(); //TODO:

} // aes::mod

#endif //AES_MODES_H
