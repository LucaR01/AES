//
// Created by Luca on 09/12/2022.
//

#include <cstring>

#include "core/aes.hpp"

#include "logger/logger.hpp"

namespace aes {

// ENCRYPTION

//TODO: volendo usare la classe Matrix e SquareMatrix.

void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t* key)
{
    //AES_DEBUG("add_round_key() | *key: {}", *key) //TODO: remove
    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            //AES_DEBUG("add_round_key() | state[i][j]: {}", state[i][j]) //TODO: uncomment or remove?
            //AES_DEBUG("add_round_key() | key[i + aes::BLOCK_WORDS * j]: {}", key[i + aes::BLOCK_WORDS * j]) //TODO: remove
            state[i][j] = state[i][j] ^ key[i + aes::BLOCK_WORDS * j];
        }
    }
}

/*void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const auto& keys) //TODO: uncomment
{
    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = state[i][j] ^ keys[i + aes::BLOCK_WORDS * j];
        }
    }
}*/

void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t& key) //TODO: uncomment
{
    for(unsigned short i = 0; i < aes::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = state[i][j] ^ key;
        }
    }
}

void add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const std::vector<uint8_t>& key)
{
    for(unsigned short i = 0; i < aes::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = state[i][j] ^ key.at(i + aes::BLOCK_WORDS * j);
        }
    }
}

void sub_bytes(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    for(auto& s : state) { //TODO: uncomment
        for(uint8_t& i : s) {
            i = S_BOX[i];
        }
    }
}

//TODO: update
void shift_row(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const unsigned short& row, const unsigned short& positions)
{
    std::array<uint8_t, aes::BLOCK_WORDS> temp{};
    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        temp[i] = state[row][(i + positions) % aes::BLOCK_WORDS]; //TODO: prima senza: % aes::BLOCK_WORDS
    }
    state[row] = temp;
}

void shift_rows(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    shift_row(state, aes::FIRST_SHIFT_ROW, 1); //TODO: magari creare delle costanti per questi? tipo: FIRST_ROW, SECOND_ROW, THIRD_ROW e SHIFT_FIRST_ROW oppure SHIFT_ONE_POSITION
    shift_row(state, aes::SECOND_SHIFT_ROW, 2); //TODO: usare una tuple? o magari un pair; ma in realtà è un singolo valore, quindi non serve un pair.
    shift_row(state, aes::THIRD_SHIFT_ROW, 3);
}

void mix_columns(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> temp{};

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; ++i) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; ++j) {
            for(uint8_t k = 0; k < aes::BLOCK_WORDS; ++k) {
                if(CIRCULANT_MDS[i][j] == 1) {
                    temp[i][k] ^= state[j][k];
                } else {
                    temp[i][k] ^= gal::galois_multiplication(CIRCULANT_MDS[i][j], state[j][k]);
                }
            }
        }
    }

    state = temp;
}

//TODO: const std::vector<uint8_t>& keys non mi va.
void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& keys, const AES& aes) //TODO: uncomment
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short number_of_rounds = aes::get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys); //TODO: .data(); .at(0);

    for(uint8_t round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys.at(round * aes::BLOCK_WORDS * aes::BLOCK_WORDS));
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys.at(number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS));

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

//TODO: stavo utilizzando questo
void encrypt_block(const std::vector<uint8_t>& input, uint8_t* output, const uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short number_of_rounds = aes::get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(uint8_t round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

/*void encrypt_block(const std::vector<uint8_t>& input, auto output, const auto& keys, const AES& aes) //TODO: uncomment
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short number_of_rounds = aes::get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(uint8_t round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}*/

void encrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(uint8_t round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

void encrypt_block(const uint8_t input[], uint8_t output[], const uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(uint8_t round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

// DECRYPTION

void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t* keys)
{
    add_round_key(state, keys);
}

void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const uint8_t& keys)
{
    add_round_key(state, keys);
}

void inverse_add_round_key(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state, const std::vector<uint8_t>& keys)
{
    add_round_key(state, keys);
}

void inverse_sub_bytes(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    for(auto& s : state) {
        for(unsigned char& i : s) {
            i = INVERSE_S_BOX[i];
        }
    }
}

void inverse_shift_rows(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    shift_row(state, aes::FIRST_SHIFT_ROW, aes::BLOCK_WORDS - 1);
    shift_row(state, aes::SECOND_SHIFT_ROW, aes::BLOCK_WORDS - 2);
    shift_row(state, aes::THIRD_SHIFT_ROW, aes::BLOCK_WORDS - 3);
}

void inverse_mix_columns(std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS>& state)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> temp{};

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; ++i) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; ++j) {
            for(uint8_t k = 0; k < aes::BLOCK_WORDS; ++k) {
                temp[i][k] ^= gal::galois_multiplication(INVERSE_CIRCULANT_MDS_MATRIX[i][j], state[j][k]);
            }
        }
    }

    state = temp;
}

//TODO: decrypt_block( vector, vector, vector, AES)
//TODO: stavo utilizzando questo, quando nella encrypt_ECB e decrypt usavo uint8_t* round_keys
void decrypt_block(const std::vector<uint8_t>& input, uint8_t* output, const uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

/*void decrypt_block(const std::vector<uint8_t>& input, auto output, const auto& keys, const AES& aes) //TODO: uncomment
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}*/

void decrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

void decrypt_block(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys.at(number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS));

    for(uint8_t round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys.at(round * aes::BLOCK_WORDS * aes::BLOCK_WORDS));
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys); //TODO: .data(); .at(0);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

void decrypt_block(const uint8_t input[], uint8_t output[], uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, aes::BLOCK_WORDS>, aes::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            state[i][j] = input[i + aes::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * aes::BLOCK_WORDS * aes::BLOCK_WORDS);

    for(uint8_t round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * aes::BLOCK_WORDS * aes::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(uint8_t i = 0; i < aes::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < aes::BLOCK_WORDS; j++) {
            output[i + aes::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

// KEY EXPANSION | KEY SCHEDULE
void key_expansion(const std::vector<uint8_t>& key, std::vector<uint8_t>& word, const AES& aes)
{
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> rcon{};

    const unsigned short& number_of_rounds = get_number_of_rounds(aes);
    const unsigned short& number_of_keys = get_number_of_keys(aes);

    for(unsigned int i = 0; i < aes::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned int j = aes::AES_128_NUMBER_OF_KEYS * number_of_keys; j < aes::AES_128_NUMBER_OF_KEYS * aes::BLOCK_WORDS * (number_of_rounds + 1); j += aes::AES_128_NUMBER_OF_KEYS) {
        temp[0] = word[j - aes::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - aes::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - aes::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - aes::AES_128_NUMBER_OF_KEYS + 3];

        if(j / aes::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * aes::AES_128_NUMBER_OF_KEYS));

            for(uint8_t k = 0; k < aes::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > aes::AES_192_NUMBER_OF_KEYS && j / aes::AES_128_NUMBER_OF_KEYS % number_of_keys == aes::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}

void key_expansion(const std::vector<uint8_t>& key, uint8_t* word, const AES& aes) //TODO: uncomment
{
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> rcon{};

    const unsigned short& number_of_keys = aes::get_number_of_keys(aes);
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    for(unsigned short i = 0; i < aes::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned int j = aes::AES_128_NUMBER_OF_KEYS * number_of_keys; j < aes::AES_128_NUMBER_OF_KEYS * aes::BLOCK_WORDS * (number_of_rounds + 1); j += aes::BLOCK_WORDS) { //TODO: aes::AES_128_keys
        temp[0] = word[j - aes::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - aes::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - aes::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - aes::AES_128_NUMBER_OF_KEYS + 3];

        if(j / aes::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * aes::AES_128_NUMBER_OF_KEYS));

            for(unsigned short k = 0; k < aes::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > aes::AES_192_NUMBER_OF_KEYS && j / aes::AES_128_NUMBER_OF_KEYS % number_of_keys == aes::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}

/*void key_expansion(const std::vector<uint8_t>& key, auto word, const AES& aes) //TODO: uncomment
{
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> rcon{};

    const unsigned short& number_of_keys = aes::get_number_of_keys(aes);
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    for(unsigned short i = 0; i < aes::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned int j = aes::AES_128_NUMBER_OF_KEYS * number_of_keys; j < aes::AES_128_NUMBER_OF_KEYS * aes::BLOCK_WORDS * (number_of_rounds + 1); j += aes::BLOCK_WORDS) { //TODO: aes::AES_128_keys
        temp[0] = word[j - aes::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - aes::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - aes::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - aes::AES_128_NUMBER_OF_KEYS + 3];

        if(j / aes::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * aes::AES_128_NUMBER_OF_KEYS));

            for(unsigned short k = 0; k < aes::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > aes::AES_192_NUMBER_OF_KEYS && j / aes::AES_128_NUMBER_OF_KEYS % number_of_keys == aes::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}*/

void key_expansion(const uint8_t key[], unsigned char word[], const AES& aes)
{
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS> rcon{};

    /*for(unsigned short i = 0; i < aes::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) { //TODO: original
        word[i] = key[i];
    }*/

    const unsigned short& number_of_keys = aes::get_number_of_keys(aes);
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    unsigned int i = 0;
    while(i < aes::AES_128_NUMBER_OF_KEYS * number_of_keys) {
        word[i] = key[i];
        i++;
    }

    i = aes::BLOCK_WORDS * number_of_keys;
    while(i < aes::AES_128_NUMBER_OF_KEYS * aes::BLOCK_WORDS * (number_of_rounds + 1)) {
        temp[0] = word[i - aes::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[i - aes::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[i - aes::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[i - aes::AES_128_NUMBER_OF_KEYS + 3];

        if (i / aes::AES_128_NUMBER_OF_KEYS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, i / (number_of_keys * aes::AES_128_NUMBER_OF_KEYS));
            for (unsigned short k = 0; k < aes::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: usare galois_add_sub()?
            }
        } else if (number_of_keys > aes::AES_192_NUMBER_OF_KEYS &&
                   i / aes::AES_128_NUMBER_OF_KEYS % number_of_keys == aes::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[i + 0] = word[i - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: usare galois_add_sub()?
        word[i + 1] = word[i + 1 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[i + 2] = word[i + 2 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[i + 3] = word[i + 3 - aes::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];

        i += 4;
    }
}

void rot_word(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys)
{
    //TODO: magari utilizzare un for
    const uint8_t temp = keys[0]; //TODO: static non andrebbe!
    keys[0] = keys[1];
    keys[1] = keys[2];
    keys[2] = keys[3];
    keys[3] = temp;
}

void sub_word(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys)
{
    for(uint8_t i = 0; i < aes::AES_128_NUMBER_OF_KEYS; i++) {
        keys[i] = S_BOX[keys[i]];
    }
}

void rcon(std::array<uint8_t, aes::AES_128_NUMBER_OF_KEYS>& keys, const uint8_t& number_of_keys)
{
    uint8_t temp = 1;
    for(uint8_t i = 0; i < number_of_keys - 1; i++) {
        temp = gal::xtime(temp); //TODO: warning, rimuovendo il constexpr si fixa.
    }

    keys[0] = temp;
    keys[1] = keys[2] = keys[3] = 0;
}

void xor_blocks(const uint8_t* x, const uint8_t* y, uint8_t* z, const unsigned int& block_length) //TODO: riscrivere?
{
    for(unsigned int i = 0; i < block_length; i++) {
        z[i] = x[i] ^ y[i]; //TODO: usare galois_add_sub()?
    }
}

} // namespace aes
