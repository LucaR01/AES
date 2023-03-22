//
// Created by Luca on 10/02/2023.
//

#ifndef AES_AES_RANDOM_HPP
#define AES_AES_RANDOM_HPP

#include <vector>

namespace aes::rnd {

/**
 * @return: an unsigned int which cannot be discarded [[nodiscard]] which is the seed of our random generator.
 */
[[nodiscard]] unsigned int get_seed();

/**
 *
 * @return: a char made by our random generator.
 */
[[nodiscard]] char get_random_byte();

[[nodiscard]] std::vector<char> get_random_bytes(const unsigned int& number_of_random_bytes);

} // namespace aes::rnd

#endif //AES_AES_RANDOM_HPP
