//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GALOIS_MATH_H
#define AES_GALOIS_MATH_H

namespace aes::gal {

static constexpr unsigned short BLOCK_SIZE = 16;

static constexpr unsigned short AES_128 = 128;
static constexpr unsigned short AES_192 = 192;
static constexpr unsigned short AES_256 = 256;

static constexpr unsigned short ROUNDS_AES_128 = 10;
static constexpr unsigned short ROUNDS_AES_192 = 12;
static constexpr unsigned short ROUNDS_AES_256 = 14;

static constexpr unsigned short MIX_COLUMNS_IRREDUCIBLE = 0x1B; // equivalente a 27 = 00011011 = x^4 + x^3 + x + 1.

//TODO: aggiungere la s-box

//TODO: da aggiornare le etichette e implementare le funzioni.
unsigned long galois_addition_subtraction(const unsigned long x, const unsigned long y);

void galois_multiplication();

void galois_division();

void galois_exponentiation();

void galois_log();

}

#endif //AES_GALOIS_MATH_H
