//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GALOIS_MATH_H
#define AES_GALOIS_MATH_H

#include <cstddef> //TODO: remove se non uso std::byte.
#include <cstdint>

namespace aes::gal {

static constexpr uint8_t BLOCK_WORDS = 4; //TODO: rename
static constexpr unsigned short BLOCK_SIZE = 16; //TODO: uint8_t

static constexpr unsigned short AES_128 = 128; //TODO: usare l'enum?
static constexpr unsigned short AES_192 = 192;
static constexpr unsigned short AES_256 = 256;

//TODO: usare uint8_t?
static constexpr unsigned short ROUNDS_AES_128 = 10; //TODO: Creare un enum? Oppure metterlo nell'enum Aes?
static constexpr unsigned short ROUNDS_AES_192 = 12; //TODO: spostarli in aes.hpp?
static constexpr unsigned short ROUNDS_AES_256 = 14;

//TODO: prima era unsigned short; metterli tuti a uint8_t
static constexpr unsigned short AES_128_NUMBER_OF_KEYS = 4; //TODO: spostarlo in galois_math.hpp? fare un enum?
static constexpr unsigned short AES_192_NUMBER_OF_KEYS = 6;
static constexpr unsigned short AES_256_NUMBER_OF_KEYS = 8;

//TODO: usare std::bitset?
static constexpr unsigned short IRREDUCIBLE_POLYNOMIAL = 0x1B; // equivalente a 27 = 00011011 = x^4 + x^3 + x + 1.

//TODO: usare std::byte o uint8_t?

//TODO: aggiungere la s-box

//TODO: aggiornare in galois_add_sub?
//TODO: static?

//TODO: se voglio metterle constexpr le devo definire nel .hpp non nel .cpp

[[nodiscard("Pure function")]] static constexpr inline uint8_t galois_addition_subtraction(const uint8_t& x, const uint8_t& y) noexcept
{
    return x ^ y;
}

[[nodiscard]] uint8_t galois_multiplication(uint8_t x, uint8_t y) noexcept;

//TODO: inline?
[[nodiscard]] static constexpr inline uint8_t xtime(const uint8_t& x) noexcept
{
    return (x << 0x01) ^ (((x >> 0x07) & 0x01) * gal::IRREDUCIBLE_POLYNOMIAL);
}

}

#endif //AES_GALOIS_MATH_H
