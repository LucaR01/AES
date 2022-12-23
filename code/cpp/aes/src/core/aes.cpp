//
// Created by Luca on 09/12/2022.
//

#include "core/aes.hpp"
#include <cstring>

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

void encrypt_block(std::string& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, uint8_t& number_of_rounds,
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

}
