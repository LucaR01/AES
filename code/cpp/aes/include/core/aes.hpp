//
// Created by Luca on 09/12/2022.
//

#ifndef AES_AES_H
#define AES_AES_H

#include <cstdint>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <optional>
#include <map>
#include <set>
#include <type_traits>

#include "math/galois_math.hpp"

namespace aes {

static constexpr uint8_t BLOCK_WORDS = 4; //TODO: rename in AES_BLOCK_WORDS?
static constexpr uint8_t BLOCK_SIZE = 16; //TODO: rename in AES_BLOCK_SIZE?

static constexpr uint8_t ROUNDS_AES_128 = 10;
static constexpr uint8_t ROUNDS_AES_192 = 12;
static constexpr uint8_t ROUNDS_AES_256 = 14;

static constexpr uint8_t AES_128_NUMBER_OF_KEYS = 4;
static constexpr uint8_t AES_192_NUMBER_OF_KEYS = 6;
static constexpr uint8_t AES_256_NUMBER_OF_KEYS = 8;

static constexpr uint8_t FIRST_SHIFT_ROW = 1;
static constexpr uint8_t SECOND_SHIFT_ROW = 2;
static constexpr uint8_t THIRD_SHIFT_ROW = 3;

//static constexpr uint8_t SHIFT_FIRST_ROW_POSITIONS = 1; //TODO: remove

enum class AES {
    AES_128 = 128,
    AES_192 = 192,
    AES_256 = 256
};

static constexpr uint8_t NUM_OF_AES_TYPES = 3;

static constexpr std::array<AES, NUM_OF_AES_TYPES> ALL_AES_TYPES = {AES::AES_128, AES::AES_192, AES::AES_256 };

//TODO: rename in AES_TYPES_NAMES o AES_NAMES.
static const std::map<AES, std::string_view>& ALL_AES_TYPES_NAMES = {
        {AES::AES_128, "AES 128"},
        {AES::AES_192, "AES 192"},
        {AES::AES_256, "AES 256"}
};

//TODO: posso templetizzare tutti i get_index dei vari enums da poi mettere in un folder generic_templates o generics o templates
/**
 * @brief This function returns the index of an AES enum instance.
 * @param aes: an element of the AES enum
 * @return the number of the index. It must be unsigned short because the last index is 256 (while the max of unsigned char is 256 - 1)
 */
[[nodiscard]] static constexpr unsigned short get_aes_index(const AES& aes)
{
    return static_cast<std::underlying_type_t<AES>>(aes);
}

static const std::set<unsigned short> AES_VALUES = { get_aes_index(AES::AES_128), get_aes_index(AES::AES_192), get_aes_index(AES::AES_256) };

/**
 * @brief This function returns the corresponding element of the enum AES based on a number.
 * @param type: a number corresponding to the AES type
 * @return the corresponding AES type as an enum
 */
[[nodiscard]] static constexpr AES get_aes_type_from_number(const unsigned short& type) //TODO: remove?
{
    switch(type) {
        case 128:
            return aes::AES::AES_128;
        case 192:
            return aes::AES::AES_192;
        case 256:
            return aes::AES::AES_256;
    }
}

/**
 *  @brief This function returns the number of rounds based on the type of AES.
 * @param aes: an element of the AES enum.
 * @return the number of rounds.
 */
[[nodiscard]] static constexpr uint8_t get_number_of_rounds(const AES& aes)
{
    switch(aes) {
        case AES::AES_192:
            return aes::ROUNDS_AES_192;
        case AES::AES_256:
            return aes::ROUNDS_AES_256;
        case AES::AES_128:
        default:
            return aes::ROUNDS_AES_128;
    }
}

/**
 *
 * @param aes: an element of the AES enum.
 * @return the number of keys correspoding to the specific AES type.
 */
[[nodiscard]] static constexpr uint8_t get_number_of_keys(const AES& aes)
{
    switch(aes) {
        case AES::AES_192:
            return aes::AES_192_NUMBER_OF_KEYS;
        case AES::AES_256:
            return aes::AES_256_NUMBER_OF_KEYS;
        case AES::AES_128:
        default:
            return aes::AES_128_NUMBER_OF_KEYS;
    }
}

static constexpr std::array<uint8_t, get_aes_index(aes::AES::AES_256)> S_BOX = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static constexpr std::array<uint8_t, get_aes_index(aes::AES::AES_256)> INVERSE_S_BOX = {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static constexpr std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> CIRCULANT_MDS = {
            {{0x02, 0x03, 0x01, 0x01},
            {0x01, 0x02, 0x03, 0x01},
            {0x01, 0x01, 0x02, 0x03},
            {0x03, 0x01, 0x01, 0x02}}
};

static constexpr std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> INVERSE_CIRCULANT_MDS_MATRIX = {
        {{0x0E, 0x0B, 0x0D, 0x09},
         {0x09, 0x0E, 0x0B, 0x0D},
         {0x0D, 0x09, 0x0E, 0x0B},
         {0x0B, 0x0D, 0x09, 0x0E}}

};

// ENCRYPTION
/**
 * @brief This function adds the round keys to the state matrix.
 * @param state: the state matrix.
 * @param keys: the round keys.
 */
void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t* keys);

void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t& key); //TODO:

void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const std::vector<uint8_t>& key);

//void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const auto& keys); //TODO: uncomment

/**
 * @brief It transforms the state matrix, in place, with the S-BOX.
 * @param state: the state matrix.
 */
void sub_bytes(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

/**
 * @brief This function shift a row of # positions.
 * @param state: the state matrix
 * @param row: the row to shift
 * @param positions: the number of positions to shift.
 */
void shift_row(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const unsigned short& row, const unsigned short& positions);

/**
 * @brief it shifts the state, the second row of 1 position, the third row of 2 positions and the fourth of 3 positions.
 * @param state: the state matrix.
 */
void shift_rows(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

/**
 * @brief It mixes the columns to improve confusion and diffusion.
 * @param state: the state matrix.
 */
void mix_columns(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& keys, const AES& aes);

//TODO: avevo fatto std::vector output e andava prima
void encrypt_block(const std::vector<uint8_t>& input, uint8_t* output, const uint8_t* keys, const AES& aes); //TODO: const uint8_t& keys ma poi non va nella key expansion

//void encrypt_block(const std::vector<uint8_t>& input, auto output, const auto& keys, const AES& aes); //TODO: uncomment

void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const uint8_t* keys, const AES& aes);

/**
 * @brief It encrypts a block of data.
 * @param input: the input.
 * @param output: the output.
 * @param keys: the round keys.
 * @param aes: the type of AES: 128, 192 or 256.
 */
void encrypt_block(const uint8_t input[], uint8_t output[], const uint8_t* keys, const AES& aes);

// DECRYPTION
/**
 * @brief It's the opposite of the add_round_key which is just the add_round_key itself.
 * @param state: the state matrix.
 * @param keys: the round keys.
 */
void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t* keys);

void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t& keys);

void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const std::vector<uint8_t>& keys);

/**
 * @brief It reverses the operation of the sub_bytes, by transforming the state, in place, with the INVERSE_S_BOX
 * @param state
 */
void inverse_sub_bytes(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

/**
 * @brief It reverses the shift of the rows respect the shift_rows. Second row: 3 positions; Third row: 2 positions; Fourth row: 1 position.
 * @param state: the state matrix.
 */
void inverse_shift_rows(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

/**
 * @brief It reverses the mix of the columns.
 * @param state: the state matrix.
 */
void inverse_mix_columns(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state);

/**
 * @brief It decrypts a block of data.
 * @param input: the input of the message.
 * @param output: the output of the decryption.
 * @param keys: the round keys.
 * @param aes: the AES enum type: 128, 192 or 256.
 */
void decrypt_block(const std::vector<uint8_t>& input, uint8_t* output, const uint8_t* keys, const AES& aes);

//void decrypt_block(const std::vector<uint8_t>& input, auto output, const auto& keys, const AES& aes); //TODO: uncomment

void decrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const uint8_t* keys, const AES& aes);

void decrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& keys, const AES& aes);

void decrypt_block(const uint8_t input[], uint8_t output[], uint8_t* keys, const AES& aes);

// KEY EXPANSION | KEY SCHEDULE

void key_expansion(const std::vector<uint8_t>& key, std::vector<uint8_t>& word, const AES& aes); //TODO: uncomment?

/**
 * @brief It's used to expand a single key into a number of round keys.
 * @param key: the single provided key.
 * @param word: the word.
 * @param aes: the type of AES enum: 128, 192 or 256.
 */
void key_expansion(const std::vector<uint8_t>& key, uint8_t* word, const AES& aes); //TODO: uncomment

//void key_expansion(const std::vector<uint8_t>& key, auto word, const AES& aes); //TODO: uncomment

void key_expansion(const uint8_t key[], unsigned char word[], const AES& aes);

/**
 * @brief It rotates the round keys.
 * @param keys: the round keys.
 */
void rot_word(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys);

/**
 * @brief It substitutes a 32 bit word using the S-BOX.
 * @param keys: the round keys.
 */
void sub_word(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys);

/**
 * @brief The round constants are generated.
 * @param keys: the round keys.
 * @param number_of_keys: based on the type of AES: 128, 192 or 256.
 */
void rcon(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys, const uint8_t& number_of_keys);

/**
 * @brief This function xor x by y by block_length times.
 * @param x: key.
 * @param y: key.
 * @param z: resulting key of the xor between x and y.
 * @param block_length: the length of a single block of data.
 */
void xor_blocks(const uint8_t* x, const uint8_t* y, uint8_t* z, const unsigned int& block_length); //TODO: riscrivere?

} // namespace aes

#endif //AES_AES_H
