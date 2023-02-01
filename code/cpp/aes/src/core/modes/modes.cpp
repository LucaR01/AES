//
// Created by Luca on 14/12/2022.
//

#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>

#include "core/modes/modes.hpp"
#include "logger/logger.hpp"

namespace aes::mod {

/*uint8_t* encrypt_ECB(const uint8_t input[], const uint8_t key[], const aes::AES& aes)
{
    unsigned short number_of_rounds = aes::get_number_of_rounds(aes);
    uint8_t* output = new unsigned char[aes::BLOCK_SIZE];
    uint8_t* round_keys = new unsigned char[4 * 4 * (number_of_rounds + 1)];
    key_expansion(key, round_keys);
    for(unsigned short i = 0; i < aes::BLOCK_SIZE; i++) {
        encrypt_block(input + i, output + i, round_keys);
    }

    delete[] round_keys;

    return output;
}

uint8_t* decrypt_ECB(const uint8_t input[], const uint8_t key[], const aes::AES& aes)
{
    unsigned short number_of_rounds = get_number_of_rounds(aes);
    uint8_t* output = new uint8_t[aes::BLOCK_SIZE];
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, number_of_rounds);
    for(unsigned short i = 0; i < aes::BLOCK_SIZE; i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input + i, output + i, round_keys, aes);
    }

    delete[] round_keys;

    return output;
}*/

//TODO: funziona, ma è da fattorizzare.
/*uint8_t* encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    unsigned short number_of_rounds = aes::get_number_of_rounds(aes);
    //AES_DEBUG("number_of_rounds: {}", number_of_rounds)
    //static constexpr unsigned short array_size = aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1);
    //std::array<uint8_t, array_size> round_keys;
    //std::vector<uint8_t> round_keys;
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    //std::vector<uint8_t>* output; //TOOD: in realtà std::array<uint8_t, 17> output{};
    //uint8_t* output{};
    //auto* output{};
    uint8_t* output = new uint8_t[aes::BLOCK_SIZE];
    aes::key_expansion(key, round_keys, aes);
    for(unsigned short i = 0; i < aes::BLOCK_SIZE; i += aes::BLOCK_SIZE) {
        //aes::encrypt_block(input.at(i) + i, output + i, round_keys, aes);
        //aes::encrypt_block(input, output, round_keys, aes);
        aes::encrypt_block(input, output + i, round_keys, aes); //TODO: forse non serve input + i
    }

    delete[] round_keys;

    return output;
}

//TODO: funziona, ma è da fattorizzare.
uint8_t* decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);
    //AES_DEBUG("number_of_rounds: {}", number_of_rounds)
    uint8_t* output = new uint8_t[aes::BLOCK_SIZE]; //TODO:
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    for(unsigned short i = 0; i < aes::BLOCK_SIZE; i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input, output + i, round_keys, aes); //TODO: forse non serve input + i
    }

    delete[] round_keys;

    return output;
}*/

// ------------------------------------------------------------------------------------------

//TODO: usare smart pointers

uint8_t* encrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes)
{
    verify_length(input_length);

    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);
    uint8_t* output = new unsigned char[input_length];
    uint8_t* round_keys = new unsigned char[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_WORDS * aes::BLOCK_WORDS * sizeof(unsigned char)) {
        aes::encrypt_block(input + i, output + i, round_keys, aes);
    }

    delete[] round_keys;

    return output;
}

uint8_t* decrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes)
{
    verify_length(input_length);

    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);
    uint8_t* output = new unsigned char[input_length];
    uint8_t* round_keys = new unsigned char[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_WORDS * aes::BLOCK_WORDS * sizeof(unsigned char)) {
        aes::decrypt_block(input + i, output + i, round_keys, aes);
    }

    delete[] round_keys;

    return output;
}

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes) //TODO: uncomment both
{
    uint8_t* output = aes::mod::encrypt_ECB(input.data(), input.size(), key.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(unsigned char));
    delete[] output;

    return vec;
}

std::vector<uint8_t> decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes)
{
    uint8_t* output = aes::mod::decrypt_ECB(input.data(), input.size(), key.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(unsigned char));
    delete[] output;

    return vec;
}

void verify_length(const unsigned int& size)
{
    if(size % aes::BLOCK_SIZE != 0) {
        //TODO: se non lo mostra nella console, fare un std::cout o std::cerr
        throw std::length_error("Il messaggio deve essere divisibile per " + std::to_string(aes::BLOCK_SIZE)); //TODO: si potrebbe aggiungere il messaggio (input)
    }
}

// ------------------------------------------------------------------------------------------

/*std::vector<uint8_t> encrypt_ECB2(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes)
{
    verify_length(input.size());
    std::vector<uint8_t> output;
    std::vector<uint8_t> round_keys;

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) {
        aes::encrypt_block(input, output, round_keys, aes); //TODO: prima input.at(i), output.at(i)
    }

    //aes::encrypt_block(input, output, round_keys, aes); //TODO: remove

    return output;
}

std::vector<uint8_t> decrypt_ECB2(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes)
{
    verify_length(input.size());
    std::vector<uint8_t> output;
    std::vector<uint8_t> round_keys;

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) { //TODO: a cosa serve la i in questo loop?
        aes::decrypt_block(input, output, round_keys, aes);
    }

    //aes::decrypt_block(input, output, round_keys, aes); //TODO: remove

    return output;
}*/

//TODO: uncomment and retry
/*std::shared_ptr<uint8_t> encrypt_ECB3(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes)
{
    verify_length(input_length);

    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);
    std::shared_ptr<uint8_t> output; //TODO: prima = new unsigned char[input_length]
    std::shared_ptr<uint8_t> round_keys; //TODO: prima = new unsigned char[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys.get(), aes);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_WORDS * aes::BLOCK_WORDS * sizeof(unsigned char)) {
        aes::encrypt_block(input + i, output.get() + i, round_keys.get(), aes);
    }

    //delete[] round_keys;

    return output;
}

std::shared_ptr<uint8_t> decrypt_ECB3(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes)
{
    verify_length(input_length);

    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);
    std::shared_ptr<uint8_t> output; //TODO: prima = new unsigned char[input_length];
    std::shared_ptr<uint8_t> round_keys; //TODO: prima era = new unsigned char[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys.get(), aes);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_WORDS * aes::BLOCK_WORDS * sizeof(unsigned char)) {
        aes::decrypt_block(input + i, output.get() + i, round_keys.get(), aes);
    }

    //delete[] round_keys;

    return output;
}

std::vector<uint8_t> encrypt_ECB3(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes)
{
    std::shared_ptr<uint8_t> output = aes::mod::encrypt_ECB3(input.data(), input.size(), key.data(), aes);
    std::vector<uint8_t> vec(output.get(), output.get() + input.size() * sizeof(unsigned char));
    //delete[] output;

    return vec;
}

std::vector<uint8_t> decrypt_ECB3(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes)
{
    std::shared_ptr<uint8_t> output = aes::mod::decrypt_ECB3(input.data(), input.size(), key.data(), aes);
    std::vector<uint8_t> vec(output.get(), output.get() + input.size() * sizeof(unsigned char));
    //delete[] output;

    return vec;
}*/

} // namespace aes::mod
