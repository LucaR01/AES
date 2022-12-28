//
// Created by Luca on 14/12/2022.
//

#include <iostream>
#include <type_traits>
#include <utility>

#include "core/modes/modes.hpp"
#include "core/aes.hpp"

namespace aes::mod {

unsigned short get_mode_index(const Modes& mode)
{
    return static_cast<std::underlying_type_t<Modes>>(mode);
}

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const uint16_t& number_of_rounds )
{
    //C++20 std::to_array();
    // std::span
    //std::array<uint8_t, input.size()> output{};
    std::vector<uint8_t> output;
    std::vector<uint8_t> round_keys;
    //std::array output{};
    //std::array<uint8_t, 4 * 4 * number_of_rounds + 1> round_keys{};
    for(unsigned int i = 0; i < input.size(); i++) {
        //aes::encrypt_block(input.data() + i, output.data() + i, round_keys, number_of_rounds ); //TODO:
    }

    return output;
}

}
