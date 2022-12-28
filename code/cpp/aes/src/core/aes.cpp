//
// Created by Luca on 09/12/2022.
//

#include "core/aes.hpp"

namespace aes {

// ENCRYPTION

//TODO: volendo usare la classe Matrix e SquareMatrix.

void add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key)
{
    // Aggiungo la chiave allo stato.
    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] ^= key[i][j]; //TODO: key[i + gal::BLOCK_WORDS * j]
            //TODO: volendo potrei anche fare così state[i][j] = gal::galois_addition_subtraction(state[i][j], key[i][j]);
        }
    }
}

void sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    for(auto& i : state) {
        for(unsigned char& j : i) {
            j = S_BOX[j];
        }
    }
}

//TODO: update
void shift_row(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, unsigned short row, unsigned short positions)
{
    std::array<uint8_t, gal::BLOCK_WORDS> temp{};
    for(unsigned int i = 0; i < gal::BLOCK_WORDS; i++) {
        temp[i] = state[row][(i + positions)]; //TODO:
    }
    //std::memcpy(state[row], temp, gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
    state[row] = temp;
    //TODO: copio direttamente o con iterator o direttamente: state[row] = temp; oppure
    //TODO: std::copy(std::begin(temp.data()), std::end(temp.data()), std::begin(state[row])); #include <algorithm>
}

void shift_rows(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    shift_row(state, 1, 1);
    shift_row(state, 2, 2);
    shift_row(state, 3, 3);
}

void mix_columns(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    //TODO: se dà problemi switcharlo ad unsigned short.
    //TODO: rinominarlo in temp?
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> output{}; //TODO: initializer {}

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            //output[i][j] = 0x00; //TODO: forse questo non serve, perché viene automaticamente inizializzato a 0.
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; k++) {
                output[i][j] ^= gal::galois_multiplication(CIRCULANT_MDS[i][k], state[k][j]);
                //TODO: usare un galois multiplication lookup table al posto?
            }
        }
    }

    state = output;
    //std::memcpy(state.data(), output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
}

//TODO: prima avevo usato std::string
void encrypt_block(const std::vector<uint8_t>& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, const uint8_t& number_of_rounds,
                   std::array<uint8_t, gal::BLOCK_WORDS>& output )
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input.at(i); //TODO: input.at(i + gal::BLOCK_WORDS * j)?
        }
    }

    //TODO: usare il reinterpret_cast è pericoloso; da cambiare.
    // FIRST ROUND
    add_round_key(state, keys);

    // FIRST ROUND - LAST ROUND - 1
    for(unsigned short round = 1; round < number_of_rounds; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state,reinterpret_cast<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> &>(keys.at(round))); //TODO: modificare.
    }

    // LAST ROUND
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, reinterpret_cast<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> &>(keys.at(number_of_rounds))); //TODO: modificare.

    /*for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) { //TODO: uncomment?
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i++] = state[i][j];
        }
    }*/

    unsigned short i = 0; //TODO: volendo metterlo direttamente nel loop, come sopra.
    for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
        for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
            output[i++] = state[row][column]; //TODO: output[i + gal::BLOCK_WORDS * j]?

            //TODO: state[row][column] ^= iv[i]; oppure gal::galois_add_sub(state[row][column], iv[i]);
        }
    }
}

// DECRYPTION

void inverse_add_round_key(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& key)
{
   add_round_key(state, key);
}

void inverse_sub_bytes(std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& state)
{
    for(auto& i : state) {
        for(unsigned char& j : i) {
            j = INVERSE_S_BOX[j];
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

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            //output[i][j] = 0x00; //TODO: uncomment?
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; k++) {
                output[i][j] ^= gal::galois_multiplication(INVERSE_CIRCULANT_MDS_MATRIX[i][k], state[k][j]);
            }
        }
    }

    //std::memcpy(state, output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t)); //TODO: remove?
    state = output;
}

void decrypt_block(std::string& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, uint8_t& number_of_rounds,
                   std::array<uint8_t, gal::BLOCK_WORDS>& output )
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};

    unsigned short i = 0; //TODO: volendo metterlo direttamente nel loop?
    for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
        for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
            state[row][column] = input[i]; //TODO: volendo input[i++]? e quindi toglierlo sotto?
            i++;
        }
    }

    //TODO: usare reinterpret_cast è pericoloso! da cambiare.

    // LAST ROUND
    inverse_add_round_key(state, reinterpret_cast<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> &>(keys[number_of_rounds]));
    inverse_shift_rows(state); //TODO: remove?
    inverse_sub_bytes(state); //TODO: remove?

    // LAST ROUND - 1 --> FIRST ROUND
    for(unsigned short round = number_of_rounds - 1; round > 0; round--) {
        //TODO: cambiare ordine? nuovo ordine: inverse_sub_bytes, inverse_shift_rows, add_round_key, inverse_mix_columns?
        inverse_add_round_key(state,reinterpret_cast<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> &>(keys[round])); //TODO: modificare.
        inverse_mix_columns(state);
        inverse_shift_rows(state);
        inverse_sub_bytes(state);
    }

    // FIRST ROUND
    //TODO: aggiungere prima di inverse_add_round_key: inverse_sub_bytes, inverse_shift_rows
    inverse_add_round_key(state, reinterpret_cast<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> &>(keys[0])); //TODO: modificare.

    unsigned short j = 0; //TODO: metterlo direttamente nel loop stesso?
    for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
        for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
            output[j] = state[row][column]; //TODO: output[j++] e quindi rimuovere il j++ sotto.
            j++;

            //TODO: xor it with IV. output[j] ^= iv[j] oppure gal::galois_add_sub(output[j], iv[j])
        }
    }
}

// KEY EXPANSION | KEY SCHEDULE

//TODO: remove AES& key_length
//TODO: std::array<uint8_t, 16> key, std::array<uint8_t, 44> word;
//TODO: oppure std::vector<uint8_t> key, std::vector<uint8_t> word;
void key_expansion(std::string& key, const unsigned short& number_of_keys, std::string& word)
{
    //number of keys = Nk = 4, 6, 8

    /*switch(key_length) { //TODO: remove
        case AES::AES_192:
            //TODO: chiamare key_expansion_aes_192() o key_expansion_aes192() o key_expansion_192();
            //TODO: oppure fare tutto in questa funzione.
            //TODO: rcon(keys, AES_192_NUMBER_OF_KEYS * 4);
            break;
        case AES::AES_256:
            break;
        case AES::AES_128:
        default:
            break;
    }*/

    std::array<uint8_t, AES_128_NUMBER_OF_KEYS> temp{};
    std::array<uint8_t, AES_128_NUMBER_OF_KEYS> rcon{};

    for(unsigned short i = 0; i < AES_128_NUMBER_OF_KEYS * number_of_keys; i++) {
        word[i] = key[i];
    }

    for(unsigned short j = AES_128_NUMBER_OF_KEYS * number_of_keys; j < AES_128_NUMBER_OF_KEYS * gal::BLOCK_WORDS * (number_of_keys + 1); j += 4) {
        temp[0] = word[j - AES_128_NUMBER_OF_KEYS + 0];
        temp[1] = word[j - AES_128_NUMBER_OF_KEYS + 1];
        temp[2] = word[j - AES_128_NUMBER_OF_KEYS + 2];
        temp[3] = word[j - AES_128_NUMBER_OF_KEYS + 3];

        if(j / gal::BLOCK_WORDS % number_of_keys == 0) {
            rot_word(temp);
            sub_word(temp);
            aes::rcon(rcon, j / (number_of_keys * AES_128_NUMBER_OF_KEYS));

            for(unsigned short k = 0; k < AES_128_NUMBER_OF_KEYS; k++) { //TODO: mettere in una funzione a parte?
                temp[k] = temp[k] ^ rcon[k]; //TODO: galois_add_sub()
            }
        } else if(number_of_keys > AES_192_NUMBER_OF_KEYS && j / AES_128_NUMBER_OF_KEYS % number_of_keys == AES_128_NUMBER_OF_KEYS) {
            sub_word(temp);
        }

        word[j + 0] = word[j - AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[0]; //TODO: galois_add_sub()
        word[j + 1] = word[j + 1 - AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[1];
        word[j + 2] = word[j + 2 - AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[2];
        word[j + 3] = word[j + 3 - AES_128_NUMBER_OF_KEYS * number_of_keys] ^ temp[3];
    }
}

void rot_word(std::array<uint8_t, AES_128_NUMBER_OF_KEYS>& keys)
{
    //TODO: magari utilizzare un for
    uint8_t temp = keys[0];
    keys[0] = keys[1];
    keys[1] = keys[2];
    keys[2] = keys[3];
    keys[3] = temp;
}

void sub_word(std::array<uint8_t, AES_128_NUMBER_OF_KEYS>& keys)
{
    for(unsigned short i = 0; i < AES_128_NUMBER_OF_KEYS; i++) {
        keys[i] = S_BOX[keys[i]]; //TODO: fix? i % 16
    }
}

void rcon(std::array<uint8_t, AES_128_NUMBER_OF_KEYS>& keys, const unsigned short& number_of_keys)
{
    //Nk = number of keys = 4, 6, 8
    uint8_t temp = 1;
    for(unsigned short i = 0; i < number_of_keys - 1; i++) {
        temp = gal::xtime(temp);
    }

    keys[0] = temp;
    keys[1] = keys[2] = keys[3] = 0;
}

}
