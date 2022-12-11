//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GALOIS_MATH_H
#define AES_GALOIS_MATH_H

#include <cstddef> //TODO: remove se non uso std::byte.
#include <cstdint>

namespace aes::gal {

static constexpr unsigned short BLOCK_SIZE = 16;

static constexpr unsigned short AES_128 = 128;
static constexpr unsigned short AES_192 = 192;
static constexpr unsigned short AES_256 = 256;

static constexpr unsigned short ROUNDS_AES_128 = 10;
static constexpr unsigned short ROUNDS_AES_192 = 12;
static constexpr unsigned short ROUNDS_AES_256 = 14;

static constexpr unsigned short MIX_COLUMNS_IRREDUCIBLE = 0x1B; // equivalente a 27 = 00011011 = x^4 + x^3 + x + 1.

//TODO: usare std::byte o uint8_t?

//TODO: aggiungere la s-box

//TODO: aggiornare in galois_add_sub?
uint8_t galois_addition_subtraction(const uint8_t x, const uint8_t y);

uint8_t galois_multiplication(uint8_t x, uint8_t y);

}

#endif //AES_GALOIS_MATH_H
