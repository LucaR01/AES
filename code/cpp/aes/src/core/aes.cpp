//
// Created by Luca on 09/12/2022.
//

#include <cstring>

#include "core/aes.hpp"

#include "logger/logger.hpp"

namespace aes {

// ENCRYPTION

//TODO: volendo usare la classe Matrix e SquareMatrix.

/*void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key)
{
    // Aggiungo la chiave allo stato.
    for(uint8_t i = 0; i < gal::BLOCK_WORDS; i++) {
        for(uint8_t j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] ^= key[i][j]; //TODO: key[i + gal::BLOCK_WORDS * j]
            //TODO: volendo potrei anche fare così state[i][j] = gal::galois_addition_subtraction(state[i][j], key[i][j]);
        }
    }
}*/

void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, const uint8_t* key)
{
    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            //AES_DEBUG("add_round_key() | state[i][j]: {}", state[i][j]) //TODO: uncomment?
            state[i][j] = state[i][j] ^ key[i + gal::BLOCK_WORDS * j];
        }
    }
}

/*void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::vector<uint8_t>& keys)
{
    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = state[i][j] ^ keys[i + gal::BLOCK_WORDS * j];
        }
    }
}*/

void sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    /*for(auto& i : state) { //TODO: uncomment
        for(unsigned char& j : i) {
            j = S_BOX[j];
        }
    }*/

    unsigned char temp;

    for(int i = 0; i < 4; i++) { //TODO: gal::BLOCK_WORDS
        for(int j = 0; j < 4; j++) {
            temp = state[i][j];
            state[i][j] = S_BOX[temp];
        }
    }
}

//TODO: update
void shift_row(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, unsigned short row, unsigned short positions)
{
    std::array<uint8_t, gal::BLOCK_WORDS> temp{};
    for(uint8_t i = 0; i < gal::BLOCK_WORDS; i++) {
        temp[i] = state[row][(i + positions) % gal::BLOCK_WORDS]; //TODO: prima senza: % gal::BLOCK_WORDS
    }
    //std::memcpy(state[row], temp, gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
    state[row] = temp;
    //TODO: copio direttamente o con iterator o direttamente: state[row] = temp; oppure
    //TODO: std::copy(std::begin(temp.data()), std::end(temp.data()), std::begin(state[row])); #include <algorithm>
}

void shift_rows(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    shift_row(state, 1, 1); //TODO: magari creare delle costanti per questi.
    shift_row(state, 2, 2);
    shift_row(state, 3, 3);
}

void mix_columns(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    //TODO: se dà problemi switcharlo ad unsigned short.
    //TODO: rinominarlo in temp?
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> output{};

    /*for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) { //TODO: originale.
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            //output[i][j] = 0x00; //TODO: forse questo non serve, perché viene automaticamente inizializzato a 0.
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; k++) {
                output[i][j] ^= gal::galois_multiplication(CIRCULANT_MDS[i][k], state[k][j]);
                //TODO: usare un galois multiplication lookup table al posto?
            }
        }
    }*/

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; ++i) { //TODO: prima era i++, anche per la j e la k.
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; ++j) {
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; ++k) {
                if(CIRCULANT_MDS[i][k] == 1) {
                    output[i][j] ^= state[k][j];
                } else {
                    output[i][j] ^= gal::galois_multiplication(CIRCULANT_MDS[i][k], state[k][j]); //TODO: usare un galois multiplication lookup table al posto?
                }
            }
        }
    }

    /*for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        std::memcpy(state.data(), output.data(), gal::BLOCK_WORDS);
    }*/

    state = output;
    //std::memcpy(state.data(), output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
}

// prima era std::vector<uint8_t>& keys, std::vector<uint8_t>& output
/*void encrypt_block(const std::vector<uint8_t>& input, uint8_t* output, uint8_t* keys, const AES& aes) //TODO: uncomment
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};
    const unsigned short number_of_rounds = aes::get_number_of_rounds(aes);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input[i + gal::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(unsigned short round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * gal::BLOCK_WORDS * gal::BLOCK_WORDS); //bug: prima avevo scritto * number_of_rounds
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * gal::BLOCK_WORDS * gal::BLOCK_WORDS);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i + gal::BLOCK_WORDS * j] = state[i][j];
        }
    }
}*/

void encrypt_block(const uint8_t input[], uint8_t output[], uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};
    const unsigned short number_of_rounds = aes::get_number_of_rounds(aes);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input[i + gal::BLOCK_WORDS * j];
        }
    }

    add_round_key(state, keys);

    for(unsigned short round = 1; round <= number_of_rounds - 1; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, keys + round * gal::BLOCK_WORDS * gal::BLOCK_WORDS);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, keys + number_of_rounds * gal::BLOCK_WORDS * gal::BLOCK_WORDS);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i + gal::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

// DECRYPTION

/*void inverse_add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key)
{
   add_round_key(state, key);
}*/

void inverse_add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, const uint8_t* keys)
{
    add_round_key(state, keys);
}

void inverse_sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    /*for(auto& i : state) { //TODO: original code.
        for(unsigned char& j : i) {
            j = INVERSE_S_BOX[j];
        }
    }*/

    unsigned char temp;

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            temp = state[i][j];
            state[i][j] = INVERSE_S_BOX[temp];
        }
    }
}

void inverse_shift_rows(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    shift_row(state, 1, gal::BLOCK_WORDS - 1);
    shift_row(state, 2, gal::BLOCK_WORDS - 2);
    shift_row(state, 3, gal::BLOCK_WORDS - 3);
}

void inverse_mix_columns(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> output{};

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; ++i) { //TODO: prima era i++ e anche gli altri.
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; ++j) {
            //output[i][j] = 0x00; //TODO: uncomment?
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; ++k) {
                output[i][k] ^= gal::galois_multiplication(INVERSE_CIRCULANT_MDS_MATRIX[i][j], state[j][k]);
            }
        }
    }

    /*for(int i = 0; i < gal::BLOCK_WORDS; i++) {
        std::memcpy(state.data()[i], output.data()[i], gal::BLOCK_WORDS);
    }*/

    //std::memcpy(state, output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
    state = output;
}

/*void decrypt_block(const std::vector<uint8_t>& input, uint8_t* output, uint8_t* keys, const AES& aes) //TODO: uncomment
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input[i + gal::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * gal::BLOCK_WORDS * gal::BLOCK_WORDS);

    for(unsigned short round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * gal::BLOCK_WORDS * gal::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i + gal::BLOCK_WORDS * j] = state[i][j];
        }
    }
}*/

void decrypt_block(const uint8_t input[], uint8_t output[], uint8_t* keys, const AES& aes)
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};
    const unsigned short& number_of_rounds = get_number_of_rounds(aes);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input[i + gal::BLOCK_WORDS * j];
        }
    }

    inverse_add_round_key(state, keys + number_of_rounds * gal::BLOCK_WORDS * gal::BLOCK_WORDS);

    for(unsigned short round = number_of_rounds - 1; round >= 1; round--) {
        inverse_sub_bytes(state);
        inverse_shift_rows(state);
        inverse_add_round_key(state, keys + round * gal::BLOCK_WORDS * gal::BLOCK_WORDS);
        inverse_mix_columns(state);
    }

    inverse_sub_bytes(state);
    inverse_shift_rows(state);
    inverse_add_round_key(state, keys);

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i + gal::BLOCK_WORDS * j] = state[i][j];
        }
    }
}

//TODO: decrypt_block();

// KEY EXPANSION | KEY SCHEDULE

//TODO: remove AES& key_length
//TODO: std::array<uint8_t, 16> key, std::array<uint8_t, 44> word;
//TODO: oppure std::vector<uint8_t> key, std::vector<uint8_t> word;
//TODO: prima era std::string& key, std::string& word
//TODO: volendo const uint8_t& number_of_keys
/*void key_expansion(const std::vector<uint8_t>& key, std::vector<uint8_t>& word, const unsigned short& number_of_keys) //TODO: uncomment?
{
    //number of keys = Nk = 4, 6, 8

    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> rcon{};

    for(unsigned short i = 0; i < gal::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned int j = gal::AES_128_NUMBER_OF_KEYS * number_of_keys; j < gal::AES_128_NUMBER_OF_KEYS * gal::BLOCK_WORDS * (number_of_keys + 1); j += 4) {
        temp[0] = word[j - gal::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - gal::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - gal::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - gal::AES_128_NUMBER_OF_KEYS + 3];

        if(j / gal::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * gal::AES_128_NUMBER_OF_KEYS));

            for(unsigned short k = 0; k < gal::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > gal::AES_192_NUMBER_OF_KEYS && j / gal::AES_128_NUMBER_OF_KEYS % number_of_keys == gal::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}*/

/*void key_expansion(const std::vector<uint8_t>& key, uint8_t* word, const AES& aes) //TODO: uncomment
{
    //number of keys = Nk = 4, 6, 8

    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> rcon{};

    const unsigned short& number_of_keys = aes::get_number_of_keys(aes);
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    for(unsigned short i = 0; i < gal::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned int j = gal::AES_128_NUMBER_OF_KEYS * number_of_keys; j < gal::AES_128_NUMBER_OF_KEYS * gal::BLOCK_WORDS * (number_of_rounds + 1); j += 4) { //TODO: gal::AES_128_keys
        temp[0] = word[j - gal::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - gal::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - gal::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - gal::AES_128_NUMBER_OF_KEYS + 3];

        if(j / gal::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * gal::AES_128_NUMBER_OF_KEYS));

            for(unsigned short k = 0; k < gal::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > gal::AES_192_NUMBER_OF_KEYS && j / gal::AES_128_NUMBER_OF_KEYS % number_of_keys == gal::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}*/

void key_expansion(const uint8_t key[], unsigned char word[], const AES& aes)
{
    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS> rcon{};

    /*for(unsigned short i = 0; i < gal::AES_128_NUMBER_OF_KEYS * number_of_keys; i++) { //TODO: original
        word[i] = key[i];
    }*/

    const unsigned short& number_of_keys = aes::get_number_of_keys(aes);
    const unsigned short& number_of_rounds = aes::get_number_of_rounds(aes);

    unsigned int i = 0;
    while(i < gal::AES_128_NUMBER_OF_KEYS * number_of_keys) {
        word[i] = key[i];
        i++;
    }

    i = gal::BLOCK_WORDS * number_of_keys;
    while(i < gal::AES_128_NUMBER_OF_KEYS * gal::BLOCK_WORDS * (number_of_rounds + 1)) {
        temp[0] = word[i - gal::AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[i - gal::AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[i - gal::AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[i - gal::AES_128_NUMBER_OF_KEYS + 3];

        if (i / gal::AES_128_NUMBER_OF_KEYS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, i / (number_of_keys * gal::AES_128_NUMBER_OF_KEYS));
            for (unsigned short k = 0; k < gal::AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if (number_of_keys > gal::AES_192_NUMBER_OF_KEYS &&
                   i / gal::AES_128_NUMBER_OF_KEYS % number_of_keys == gal::AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[i + 0] = word[i - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[i + 1] = word[i + 1 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[i + 2] = word[i + 2 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[i + 3] = word[i + 3 - gal::AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];

        i += 4;
    }
}

void rot_word(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys)
{
    //TODO: magari utilizzare un for
    uint8_t temp = keys[0]; //TODO: const
    keys[0] = keys[1];
    keys[1] = keys[2];
    keys[2] = keys[3];
    keys[3] = temp;
}

void sub_word(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys)
{
    for(uint8_t i = 0; i < gal::AES_128_NUMBER_OF_KEYS; i++) {
        keys[i] = S_BOX[keys[i]]; //TODO: fix? i % 16 (gal::BLOCK_SIZE)
    }
}

void rcon(std::array<uint8_t, gal::AES_128_NUMBER_OF_KEYS>& keys, const unsigned short& number_of_keys)
{
    //Nk = number of keys = 4, 6, 8
    uint8_t temp = 1;
    for(unsigned int i = 0; i < number_of_keys - 1; i++) { //TODO: uint8_t i = 0;
        temp = gal::xtime(temp); //TODO: warning, rimuovendo il constexpr si fixa.
    }

    keys[0] = temp;
    keys[1] = keys[2] = keys[3] = 0;
}

} // namespace aes
