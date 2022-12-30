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
    //aes::key_expansion(key, round_keys); //TODO: uncomment
    //TODO: non input.size(), ma 17.
    for(unsigned int i = 0; i < input.size(); i++) {
        //aes::encrypt_block(input.data() + i, output.data() + i, round_keys, number_of_rounds ); //TODO: to fix and uncomment.
    }

    return output;
}

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes )
{
    //TODO: volendo static
    const uint8_t number_of_rounds = aes::get_number_of_rounds(aes);
    //const uint8_t number_of_keys = aes::get_number_of_keys(aes);

    /*switch(aes) { //TODO: remove
        case AES_192:*/
            /*number_of_rounds = gal::ROUNDS_AES_192;
            number_of_keys = AES_192_NUMBER_OF_KEYS;*/
            /*number_of_rounds = aes::get_number_of_rounds(aes);
            number_of_keys = aes::get_number_of_keys(aes);
            break;
        case AES_256:*/
            /*number_of_rounds = gal::ROUNDS_AES_256;
            number_of_keys = AES_256_NUMBER_OF_KEYS;*/
            /*number_of_rounds = aes::get_number_of_rounds(aes);
            number_of_keys = aes::get_number_of_keys(aes);
            break;
        case AES_128:
        default:*/
            /*number_of_rounds = gal::ROUNDS_AES_128;
            number_of_keys = AES_128_NUMBER_OF_KEYS;*/
            /*number_of_rounds = aes::get_number_of_rounds(aes);
            number_of_keys = aes::get_number_of_keys(aes);
            break;
    }*/

    std::vector<uint8_t> output;
    std::vector<uint8_t> round_keys;
    //std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> round_keys;

    aes::key_expansion(key, round_keys, aes::get_number_of_keys(aes));
    //TODO: non i < input.size(), ma i < 17, ovvero gal::BLOCK_SIZE + sizeof(unsigned char), ovvero 1.
    for(unsigned int i = 0; i < input.size(); i += gal::BLOCK_SIZE + 1) {
        //aes::encrypt_block(input, output, round_keys, number_of_rounds); //TDDO: uncomment and fix.
    }

    return output;
}

}
