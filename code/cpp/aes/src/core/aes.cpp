//
// Created by Luca on 09/12/2022.
//

#include "core/aes.hpp"

namespace aes {

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
    /*for(unsigned short i = 0; i < gal::BLOCK_WORDS; i++) { //TODO: remove
        for(unsigned short j = 0; j < gal::BLOCK_WORDS; j++) {
            state[i][j] = S_BOX[state[i][j]];
        }
    }*/

    for(auto& i : state) {
        for(unsigned char& j : i) {
            j = S_BOX[j];
        }
    }
}

void shift_row()
{

}

void shift_rows()
{

}

}
