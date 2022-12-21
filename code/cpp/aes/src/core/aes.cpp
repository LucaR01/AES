//
// Created by Luca on 09/12/2022.
//

#include "core/aes.hpp"
#include <cstring>

namespace aes {

// ENCRYPTION

//TODO: volendo usare la classe Matrix e SquareMatrix.

void add_round_key(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS], uint8_t (&key)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    // Aggiungo la chiave allo stato.
    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] ^= key[i][j];
            //TODO: volendo potrei anche fare così state[i][j] = gal::galois_addition_subtraction(state[i][j], key[i][j]);
        }
    }
}

//TODO: pass by value o pass by reference?
void sub_bytes(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    for(auto& i : state) {
        for(unsigned char& j : i) {
            j = S_BOX[j];
        }
    }
}

//TODO: update
void shift_row(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS], const unsigned short row, const unsigned short positions)
{
    uint8_t temp[gal::BLOCK_WORDS];
    for(unsigned int i = 0; i < gal::BLOCK_WORDS; i++) {
        temp[i] = state[row][(i + positions)]; //TODO:
    }
    std::memcpy(state[row], temp, gal::BLOCK_WORDS * sizeof(uint8_t));
}

void shift_rows(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    shift_row(state, 1, 1);
    shift_row(state, 2, 2);
    shift_row(state, 3, 3);
}

void mix_columns(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    //TODO: se dà problemi switcharlo ad unsigned short.
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> output; //TODO: initializer {}

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i][j] = 0x00; //TODO: forse questo non serve, perché viene automaticamente inizializzato a 0.
            for(unsigned short k = 0; k < gal::BLOCK_WORDS; k++) {
                output[i][j] ^= gal::galois_multiplication(CIRCULANT_MDS[i][k], state[k][j]);
            }
        }
    }

    std::memcpy(state, output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t));
}

void encrypt_block(std::string& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, uint8_t number_of_rounds,
                   std::array<uint8_t, gal::BLOCK_WORDS>& output )
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};

    for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) {
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = input.at(i);
        }
    }

    //TODO: usare il reinterpret_cast è pericoloso; da cambiare.
    // FIRST ROUND
    add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state), reinterpret_cast<uint8_t (&)[4][4]>(keys));

    // FIRST ROUND - LAST ROUND - 1
    for(unsigned short round = 1; round < number_of_rounds; round++) {
        sub_bytes(reinterpret_cast<uint8_t (&)[4][4]>(state));
        shift_rows(reinterpret_cast<uint8_t (&)[4][4]>(state));
        mix_columns(reinterpret_cast<uint8_t (&)[4][4]>(state));
        add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state), reinterpret_cast<uint8_t (&)[4][4]>(keys.at(round)));
    }

    // LAST ROUND
    sub_bytes(reinterpret_cast<uint8_t (&)[4][4]>(state));
    shift_rows(reinterpret_cast<uint8_t (&)[4][4]>(state));
    add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state),reinterpret_cast<uint8_t (&)[4][4]>(keys.at(number_of_rounds)));

    /*for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) { //TODO: uncomment?
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            output[i++] = state[i][j];
        }
    }*/

    unsigned short i = 0; //TODO: volendo metterlo direttamente nel loop, come sopra.
    for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
        for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
            output[i++] = state[row][column];
        }
    }
}

// DECRYPTION

void inverse_add_round_key(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS], uint8_t (&key)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
   add_round_key(state, key);
}

void inverse_sub_bytes(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    for(auto& i : state) {
        for(unsigned char& j : i) {
            j = INVERSE_S_BOX[j];
        }
    }
}

void inverse_shift_rows(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
{
    shift_row(state, 1, gal::BLOCK_WORDS - 1);
    shift_row(state, 2, gal::BLOCK_WORDS - 2);
    shift_row(state, 3, gal::BLOCK_WORDS - 3);
}

void inverse_mix_columns(uint8_t (&state)[gal::BLOCK_WORDS][gal::BLOCK_WORDS])
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

    std::memcpy(state, output.data(), gal::BLOCK_WORDS * gal::BLOCK_WORDS * sizeof(uint8_t));
}

void decrypt_block(std::string& input, std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>& keys, uint8_t number_of_rounds,
                   std::array<uint8_t, gal::BLOCK_WORDS>& output )
{
    std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS> state{};

    unsigned short i = 0; //TODO: volendo metterlo direttamente nel loop?
    for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
        for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
            state[row][column] = input[i];
            i++;
        }
    }

    //TODO: usare reinterpret_cast è pericoloso! da cambiare.

    // LAST ROUND
    inverse_add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state),reinterpret_cast<uint8_t (&)[4][4]>(keys[number_of_rounds]));
    inverse_shift_rows(reinterpret_cast<uint8_t (&)[4][4]>(state));
    inverse_sub_bytes(reinterpret_cast<uint8_t (&)[4][4]>(state));

    // LAST ROUND - 1 --> FIRST ROUND
    for(unsigned short round = number_of_rounds - 1; round > 0; round--) {
        inverse_add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state),reinterpret_cast<uint8_t (&)[4][4]>(keys[round]));
        inverse_mix_columns(reinterpret_cast<uint8_t (&)[4][4]>(state));
        inverse_shift_rows(reinterpret_cast<uint8_t (&)[4][4]>(state));
        inverse_sub_bytes(reinterpret_cast<uint8_t (&)[4][4]>(state));
    }

    // FIRST ROUND
    inverse_add_round_key(reinterpret_cast<uint8_t (&)[4][4]>(state), reinterpret_cast<uint8_t (&)[4][4]>(keys[0]));

    unsigned short j = 0; //TODO: metterlo direttamente nel loop stesso?
    for(unsigned short row = 0; row < gal::BLOCK_WORDS; row++) {
        for(unsigned short column = 0; column < gal::BLOCK_WORDS; column++) {
            output[j] = state[row][column]; //TODO: output[j++]
            j++;
        }
    }
}

}
