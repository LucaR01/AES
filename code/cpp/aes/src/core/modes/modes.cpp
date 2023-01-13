//
// Created by Luca on 14/12/2022.
//

#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>

#include "core/modes/modes.hpp"

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

/*std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes )
{
    //TODO: volendo static
    const uint8_t number_of_rounds = aes::get_number_of_rounds(aes);*/
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

    /*std::vector<uint8_t> output;
    std::vector<uint8_t> round_keys;*/
    //std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> round_keys;
    //std::variant<std::vector<uint8_t>, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>> round_keys;

    //TODO: permettere a key_expansion/encrypt_block di poter prendere sia una matrice sia un vettore
    //TODO: con std::variadic, std::variant
    /*aes::key_expansion(key, round_keys, aes::get_number_of_keys(aes));
    //TODO: non i < input.size(), ma i < 17, ovvero gal::BLOCK_SIZE + sizeof(unsigned char), ovvero 1.
    for(unsigned int i = 0; i < input.size(); i += gal::BLOCK_SIZE + 1) {
        //aes::encrypt_block(input, output, round_keys, number_of_rounds); //TDDO: uncomment and fix.
    }

    return output;
}*/

/*int encrypt_ECB(const std::vector<uint8_t>& key, const AES& aes, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys)
{
    unsigned short number_of_rounds = aes::get_number_of_rounds(aes); //TODO: uint8_t?
    key_expansion(key, aes, keys);*/

    //unsigned short number_of_blocks = n >> 4; //TODO: n >> 4 + 1
    //unsigned short extra = n & 0x0F;

    //unsigned short output_length = number_of_blocks * 16; // BLOCK_SIZE

    /*if(extra) {
        output_length += gal::BLOCK_SIZE;

        aes::encrypt_block(input + (number_of_blocks << 4), extra, keys, number_of_rounds, output + (number_of_blocks << 4) );
    }

    for(int i = 0; i < number_of_blocks; i++) {
        encrypt_block(input + (i << 4), gal::BLOCK_SIZE, keys, number_of_rounds, output + (i << 4));
    }*/

    /*return 0; //TODO: update; output_length
}*/

uint8_t* encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    unsigned short number_of_rounds = aes::get_number_of_rounds(aes);
    /*static constexpr unsigned short array_size = gal::BLOCK_WORDS * gal::BLOCK_WORDS * (number_of_rounds + 1);
    std::array<uint8_t, array_size> round_keys;*/
    //std::vector<uint8_t> round_keys;
    uint8_t* round_keys = new uint8_t[gal::BLOCK_WORDS * gal::BLOCK_WORDS * (number_of_rounds + 1)];

    //std::vector<uint8_t>* output; //TOOD: in realtà std::array<uint8_t, 17> output{};
    //uint8_t* output{};
    //auto* output{};
    uint8_t* output = new uint8_t[gal::BLOCK_SIZE];
    aes::key_expansion(key, round_keys, number_of_rounds);
    for(unsigned short i = 0; i < gal::BLOCK_SIZE; i += gal::BLOCK_SIZE) {
        //aes::encrypt_block(input.at(i) + i, output + i, round_keys, aes);
        //aes::encrypt_block(input, output, round_keys, aes);
        aes::encrypt_block(input, output + i, round_keys, aes); //TODO: input + i
    }

    delete[] round_keys;

    return output;
}

uint8_t* decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    unsigned short number_of_rounds = get_number_of_rounds(aes);
    uint8_t* output = new uint8_t[gal::BLOCK_SIZE]; //TODO:
    uint8_t* round_keys = new uint8_t[gal::BLOCK_WORDS * gal::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, number_of_rounds);
    for(unsigned short i = 0; i < gal::BLOCK_SIZE; i += gal::BLOCK_SIZE) {
        aes::decrypt_block(input, output + i, round_keys, aes); //TODO: input + i
    }

    delete[] round_keys;

    return output;
}

} // namespace aes::mod
