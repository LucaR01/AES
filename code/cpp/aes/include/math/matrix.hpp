//
// Created by Luca on 17/12/2022.
//

#ifndef AES_MATRIX_H
#define AES_MATRIX_H

#include <iostream>
#include <valarray>
#include <memory>

//TODO: definizioni di una classe template nei .hpp non nei .cpp

namespace aes::mat {

//TODO: usare una struct?

template<typename T> //, unsigned short/uint8_t S
class Matrix {
public:
    Matrix(unsigned short rows, unsigned short columns) : rows(rows), columns(columns) {}; //TODO: usare uint8_t?
    ~Matrix() = default;

    //TODO: aggiungere tipiche operazioni su matrici: identità, trasposizione, ecc.

    unsigned short get_rows() const
    {
        return this->rows;
    }

    unsigned short get_columns() const
    {
        return this->columns;
    }

    std::valarray<T> get_data() const
    {
        return this->data;
    }

    /*T& operator()(unsigned short row, unsigned short column) //TODO: remove
    {
        return data[std::slice(column, rows, columns)][row];

        return data[std::slice(row, columns, rows)][column];
    }*/

    /*void print_elements() //TODO: fix
    {
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) {
                std::cout << data[i][j] << std::endl;
            }
        }
    }*/

    //TODO: operator+, operator[], operator(), operator-, operator*

private:
    unsigned short rows;
    unsigned short columns;
    std::valarray<T> data;
};

typedef Matrix<unsigned char> mat; //TODO: aggiornare; uint8_t

} // aes::mat

#endif //AES_MATRIX_H
