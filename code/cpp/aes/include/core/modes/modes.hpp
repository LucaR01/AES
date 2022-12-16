//
// Created by Luca on 09/12/2022.
//

#ifndef AES_MODES_H
#define AES_MODES_H

#include <map>
#include <string_view>
#include <array>

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
unsigned short get_mode_index(const Modes& mode);

static const std::map<Modes, std::string_view> modes_names = {
        {Modes::ECB, "ECB"},
        {Modes::CBC, "CBC"},
        {Modes::CTR, "CTR"}
};

} // aes::mod

#endif //AES_MODES_H
