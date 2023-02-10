//
// Created by Luca on 10/02/2023.
//

#ifndef AES_AES_RANDOM_HPP
#define AES_AES_RANDOM_HPP

#include <vector>

namespace aes::rnd {

unsigned int get_seed();

char get_random_byte();

std::vector<char> get_random_bytes(const unsigned int& number_of_random_bytes);

} // namespace aes::rnd

#endif //AES_AES_RANDOM_HPP
