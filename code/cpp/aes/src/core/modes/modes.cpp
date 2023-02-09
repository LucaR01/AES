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
uint8_t* encrypt_ECB4(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);
    //TODO: rendere round_keys un std::vector<uint8_t>; per fare questo l'unica funzione da cambiare è l'add_round_key();
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)]; //TODO: uint8_t* e funziona
    uint8_t* output = new uint8_t[input.size()];//TODO: std::vector<uint8_t> output; uint8_t* output = new uint8_t[input.size()];

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) {
        aes::encrypt_block(input, output + i, round_keys, aes); //TODO: forse non serve input + i, output + i
    }

    delete[] round_keys;

    return output;
}

//TODO: funziona, ma è da fattorizzare.
uint8_t* decrypt_ECB4(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);
    uint8_t* output = new uint8_t[input.size()]; //TODO: std::vector<uint8_t> output; uint8_t* output = new uint8_t[input.size()];
    //TODO: rendere round_keys un std::vector<uint8_t>; per fare questo l'unica funzione da cambiare è l'add_round_key();
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)]; //TODO: uint8_t* e funziona

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input, output + i, round_keys, aes); //TODO: forse non serve input + i, output + i
    }

    delete[] round_keys;

    return output;
}

//TODO: non va
std::vector<uint8_t> encrypt_ECB5(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);
    //std::vector<uint8_t> output; // = new uint8_t[input.size()]; //TODO: std::vector<uint8_t> output; uint8_t* output = new uint8_t[input.size()];
    //std::array<uint8_t, input.size()> out{};
    uint8_t* output = new uint8_t[input.size()];
    //TODO: rendere round_keys un std::vector<uint8_t>; per fare questo l'unica funzione da cambiare è l'add_round_key();
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)]; //TODO: uint8_t* e funziona

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input, output, round_keys, aes); //TODO: forse non serve input + i, output + i
    }

    const std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));

    delete[] round_keys;

    return vec;
}

//TODO: non va
std::vector<uint8_t> decrypt_ECB5(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
{
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);
    //std::vector<uint8_t> output; // = new uint8_t[input.size()]; //TODO: std::vector<uint8_t> output; uint8_t* output = new uint8_t[input.size()];
    uint8_t* output = new uint8_t[input.size()];
    //TODO: rendere round_keys un std::vector<uint8_t>; per fare questo l'unica funzione da cambiare è l'add_round_key();
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)]; //TODO: uint8_t* e funziona

    aes::key_expansion(key, round_keys, aes);
    for(unsigned int i = 0; i < input.size(); i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input, output, round_keys, aes); //TODO: forse non serve input + i, output + i
    }

    const std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));

    delete[] round_keys;

    return vec;
}

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

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes) //TODO: uncomment both
{
    uint8_t* output = aes::mod::encrypt_ECB(input.data(), input.size(), key.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(unsigned char));
    delete[] output;

    return vec;
}

std::vector<uint8_t> decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes)
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

// ---------------------------------------------------------------------------------------------------------------------

uint8_t* encrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes)
{
    verify_length(input_length);

    const uint8_t& number_of_rounds = aes::get_number_of_rounds(aes);

    uint8_t* output = new uint8_t[input_length];
    uint8_t block[aes::BLOCK_SIZE];
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    std::memcpy(block, iv, aes::BLOCK_SIZE);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_SIZE) {
        aes::xor_blocks(block, input + i, block, aes::BLOCK_SIZE);
        aes::encrypt_block(block, output + i, round_keys, aes);
        std::memcpy(block, output + i, aes::BLOCK_SIZE);
    }

    delete[] round_keys;

    return output;
}

uint8_t* decrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes)
{
    verify_length(input_length);

    const uint8_t& number_of_rounds = aes::get_number_of_rounds(aes);

    uint8_t* output = new uint8_t[input_length];
    uint8_t block[aes::BLOCK_SIZE];
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    std::memcpy(block, iv, aes::BLOCK_SIZE);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_SIZE) {
        aes::decrypt_block(input + i, output + i, round_keys, aes);
        aes::xor_blocks(block, output + i, output + i, aes::BLOCK_SIZE);
        std::memcpy(block, input + i, aes::BLOCK_SIZE);
    }

    delete[] round_keys;

    return output;
}

std::vector<uint8_t> encrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes)
{
    uint8_t* output = mod::encrypt_CBC(input.data(), input.size(), key.data(), iv.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));
    delete[] output;

    return vec;
}

std::vector<uint8_t> decrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes)
{
    uint8_t* output = mod::decrypt_CBC(input.data(), input.size(), key.data(), iv.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));
    delete[] output;

    return vec;
}

// CFB MODE

uint8_t* encrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes)
{
    verify_length(input_length);

    const uint8_t& number_of_rounds = aes::get_number_of_rounds(aes);

    uint8_t* output = new uint8_t[input_length];
    uint8_t block[aes::BLOCK_SIZE];
    uint8_t encrypted_block[aes::BLOCK_SIZE];
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    std::memcpy(block, iv, aes::BLOCK_SIZE);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_SIZE) {
        aes::encrypt_block(block, encrypted_block, round_keys, aes);
        aes::xor_blocks(input + i, encrypted_block, output + i, aes::BLOCK_SIZE);
        std::memcpy(block, output + i, aes::BLOCK_SIZE);
    }

    delete[] round_keys;

    return output;
}

uint8_t* decrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes)
{
    verify_length(input_length);

    const uint8_t& number_of_rounds = aes::get_number_of_rounds(aes);

    uint8_t* output = new uint8_t[input_length];
    uint8_t block[aes::BLOCK_SIZE];
    uint8_t encrypted_block[aes::BLOCK_SIZE];
    uint8_t* round_keys = new uint8_t[aes::BLOCK_WORDS * aes::BLOCK_WORDS * (number_of_rounds + 1)];

    aes::key_expansion(key, round_keys, aes);
    std::memcpy(block, iv, aes::BLOCK_SIZE);
    for(unsigned int i = 0; i < input_length; i += aes::BLOCK_SIZE) {
        aes::encrypt_block(block, encrypted_block, round_keys, aes); //TODO:
        aes::xor_blocks(input + i, encrypted_block, output + i, aes::BLOCK_SIZE);
        std::memcpy(block, input + i, aes::BLOCK_SIZE);
    }

    delete[] round_keys;

    return output;
}

std::vector<uint8_t> encrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes)
{
    uint8_t* output = mod::encrypt_CFB(input.data(), input.size(), key.data(), iv.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));
    delete[] output;

    return vec;
}

std::vector<uint8_t> decrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes)
{
    uint8_t* output = mod::decrypt_CFB(input.data(), input.size(), key.data(), iv.data(), aes);
    std::vector<uint8_t> vec(output, output + input.size() * sizeof(uint8_t));
    delete[] output;

    return vec;
}

} // namespace aes::mod
