//
// Created by Luca on 09/12/2022.
//

#ifndef AES_AES_H
#define AES_AES_H

#include <cstdint>
#include <array>
#include <string>
#include <vector>

#include "math/galois_math.hpp"

//TODO: fare una classe?

namespace aes {

//TODO: Aggiungere una funzione che in base al tipo di AES, ti restituisce il numero di rounds?
//TODO: mettere questo enum in un altro file?
enum AES {
    AES_128 = 128,
    AES_192 = 192,
    AES_256 = 256
};

/*[[nodiscard]] static constexpr uint8_t get_number_of_rounds(const AES& aes);

[[nodiscard]] uint8_t get_number_of_keys(const AES& aes);*/

[[nodiscard]] static constexpr uint8_t get_number_of_rounds(const AES& aes)
{
    switch(aes) {
        case AES_192:
            return gal::ROUNDS_AES_192;
        case AES_256:
            return gal::ROUNDS_AES_256;
        case AES_128:
        default:
            return gal::ROUNDS_AES_128;
    }
}

[[nodiscard]] static constexpr uint8_t get_number_of_keys(const AES& aes)
{
    switch(aes) {
        case AES_192:
            return gal::AES_192_NUMBER_OF_KEYS;
        case AES_256:
            return gal::AES_256_NUMBER_OF_KEYS;
        case AES_128:
        default:
            return gal::AES_128_NUMBER_OF_KEYS;
    }
}

//TODO: forse non serve fare classi separate per AES-128, AES-256, ecc.
//TODO: basta passare
/*class AES { //TODO: remove?
public:
    AES() = default;
    ~AES() = default;
    //TODO: copy e move constructors

private:
    //TODO: state
};*/

//TODO: fare delle costanti per lo shift delle righe nello shift_rows()?

//TODO: usare unsigned char anche se uint8_t è unsigned char.
static constexpr std::array<uint8_t, gal::AES_256> S_BOX = {
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

static constexpr std::array<uint8_t, gal::AES_256> INVERSE_S_BOX = {
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

//TODO: uint8_t dava problemi e non recuperava i valori.
//TODO: usare Matrix class?
//TODO: nome circulant_mds_matrix
static constexpr std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> CIRCULANT_MDS = {
            {{0x02, 0x03, 0x01, 0x01},
            {0x01, 0x02, 0x03, 0x01},
            {0x01, 0x01, 0x02, 0x03},
            {0x03, 0x01, 0x01, 0x02}}
};

static constexpr std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> INVERSE_CIRCULANT_MDS_MATRIX = {
        {{0x0E, 0x0B, 0x0D, 0x09},
         {0x09, 0x0E, 0x0B, 0x0D},
         {0x0D, 0x09, 0x0E, 0x0B},
         {0x0B, 0x0D, 0x09, 0x0E}}

};

//TODO: usare std::valarray<uint8_t> state(BLOCK_WORDS * BLOCK_WORDS) oppure anche std::array<std::array<int, BLOCK_WORDS>, BLOCK_WORDS>?
//TODO: oppure creare una classe Matrix?
//TODO: passarli come puntatori?

// ENCRYPTION
void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key);

void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, const uint8_t* keys); //TODO: uint8_t*

void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::vector<uint8_t>& keys); //TODO: *; remove?

void sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

void shift_row(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, unsigned short row, unsigned short positions);

void shift_rows(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

void mix_columns(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

//TODO: string_view?
//TODO: std::array& keys perché poi viene dedotto che è un array con size non definito; std::vector<std::array<>>? triple array tipo arr[][][];
//TODO: uint8_t* keys
//TODO: std::array<uint8_t, gal::BLOCK_WORDS>& iv,
//TODO: prima era std::array<uint8_t, gal::BLOCK_WORDS>& output
//TODO: o fare std::array<uint8_t, gal::BLOCK_SIZE + 1 (ovvero 17)> oppure std::vector<uint8_t>& output
void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output,
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, const uint8_t& number_of_rounds);

//TODO: uint8_t* keys o uint8_t&
//void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, std::vector<uint8_t>& keys, const AES& aes);

void encrypt_block(const std::vector<uint8_t>& input, uint8_t* output, uint8_t* keys, const AES& aes);

//void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const uint8_t& keys, const uint8_t& number_of_rounds ); //TODO: remove

// DECRYPTION
void inverse_add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key);

void inverse_sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

void inverse_shift_rows(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

void inverse_mix_columns(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state);

//TODO: string_view?
//TODO: std::array& keys perché poi viene dedotto che è un array con size non definito; std::vector<std::array<>>? triple array tipo arr[][][];
//TODO: uint8_t* keys
//TODO: std::array<uint8_t, gal::BLOCK_WORDS>& iv,
void decrypt_block(std::string& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, uint8_t& number_of_rounds,
                   std::array<uint8_t, gal::BLOCK_WORDS>& output );

// KEY EXPANSION | KEY SCHEDULE

void key_expansion(const std::vector<uint8_t>& key, const aes::AES& aes, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys);

//TODO: std::array<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys
//TODO: std::array<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>, gal::ROUNDS_AES_128 + 1>& keys
void key_expansion_aes_128(const std::vector<uint8_t>& key, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys);

void key_expansion_aes_192(const std::vector<uint8_t>& key, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys);

void key_expansion_aes_256(const std::vector<uint8_t>& key, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys);

void key_expansion(const std::vector<uint8_t>& key, std::vector<uint8_t>& word, const unsigned short& number_of_keys);

void rot_word(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys);

void sub_word(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys);

void rcon(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys, const unsigned short& number_of_keys);

}

#endif //AES_AES_H
