//
// Created by Luca on 10/02/2023.
//

#include <random>
#include <algorithm> // per generate
#include <climits> // per CHAR_BIT
#include <functional> // per

#include "math/random/aes_random.hpp"

namespace aes::rnd {

[[nodiscard]] unsigned int get_seed()
{
    std::random_device random_device;
    std::mt19937 random_device_generator(random_device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution{};
    return distribution(random_device_generator);
}

//TODO: questa è da rifattorizzare
[[nodiscard]] char get_random_byte()
{
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t> random_byte_engine(get_seed()); //TODO: prima srand(time(NULL)) ma non andava.
    std::vector<char> data(1); // 1 perché ci serve un singolo byte, quindi un singolo carattere.
    std::generate(data.begin(), data.end(), std::ref(random_byte_engine));
    return data.at(0);
}

[[nodiscard]] std::vector<char> get_random_bytes(const unsigned int& number_of_random_bytes)
{
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t> random_byte_engine(get_seed());
    //std::array<uint8_t, number_of_random_bytes> data{}; //TODO: questo non è possibile.
    std::vector<char> data(number_of_random_bytes);
    std::generate(data.begin(), data.end(), std::ref(random_byte_engine));
    return data;
}

} // namespace aes::rnd
