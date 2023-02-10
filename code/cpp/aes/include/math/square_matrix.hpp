//
// Created by Luca on 17/12/2022.
//

#ifndef AES_SQUARE_MATRIX_H
#define AES_SQUARE_MATRIX_H

#include "math/matrix.hpp"

namespace aes::mat {

template<typename T, unsigned short S> //TODO: usare uint8_t?
class SquareMatrix : public Matrix<T> {
public:
    SquareMatrix() : square_matrix(Matrix<T>(S, S)) {} // std::make_shared<Matrix>(Matrix<T>(S, S))
    ~SquareMatrix() = default;

private:
    Matrix<T> square_matrix;
};

typedef SquareMatrix<unsigned char, 4> sqmat4; //TODO: uint8_t; usare gal::BLOCK_WORDS;

}

#endif //AES_SQUARE_MATRIX_H
