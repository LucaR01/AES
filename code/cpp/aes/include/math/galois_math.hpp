//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GALOIS_MATH_H
#define AES_GALOIS_MATH_H

#include <cstddef> //TODO: remove se non uso std::byte.
#include <cstdint>

namespace aes::gal {

//TODO: usare std::bitset?
static constexpr uint8_t IRREDUCIBLE_POLYNOMIAL = 0x1B; // equivalente a 27 = 00011011 = x^4 + x^3 + x + 1.

//TODO: se voglio metterle constexpr le devo definire nel .hpp non nel .cpp

/**
 * @brief This function returns the galois addition or subtraction between two numbers. It xors the two numbers.
 * @param x: a uint8_t aka unsigned char.
 * @param y: a uint8_t aka unsigned char.
 * @return an xor between x and y.
 */
[[nodiscard("Pure function")]] static constexpr inline uint8_t galois_addition_subtraction(const uint8_t& x, const uint8_t& y) noexcept
{
    return x ^ y;
}

/**
 * @brief This function returns the multiplication in the galois field of x and y.
 * @param x: a uint8_t aka unsigned char.
 * @param y: a uint8_t aka unsigned char.
 * @return the galois multiplication between x and y.
 */
[[nodiscard]] static constexpr uint8_t galois_multiplication(uint8_t x, uint8_t y) noexcept
{
    uint8_t result = 0;

    for(unsigned short i = 0; i < 8; i++) {
        if(y & 0x01) {
            result ^= x; // ^= è l'addizione in GF(2^8); potrei anche fare result = galois_addition_subtraction(result, x);
        }

        const bool high_bit = x & 0x80; // x >= 0x80 = 128
        x <<= 1; // ruotiamo x di 1 (moltiplicazione in GF(2^8)
        if(high_bit) {
            x ^= IRREDUCIBLE_POLYNOMIAL; // x -= 0x1B, ovvero mod(x^8 + x^4 + x^3 + x + 1)
        }

        y >>= 1; // ruotiamo y a destra (divisione in GF(2^8)
    }

    return result;
}

//TODO: inline?
/**
 *
 * @param x: a uint8_t aka unsigned char.
 * @return a multiplication of the number x.
 */
[[nodiscard]] static constexpr inline uint8_t xtime(const uint8_t& x) noexcept
{
    return (x << 0x01) ^ (((x >> 0x07) & 0x01) * gal::IRREDUCIBLE_POLYNOMIAL);
}

} // namespace aes::gal

#endif //AES_GALOIS_MATH_H
