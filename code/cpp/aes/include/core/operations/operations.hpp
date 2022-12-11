//
// Created by Luca on 11/12/2022.
//

#ifndef AES_OPERATIONS_H
#define AES_OPERATIONS_H

#include <iostream>
#include <map>
#include <string>

enum Operations { //TODO: rename in ENCRYPTION e DECRYPTION?
    ENCRYPT = 1,
    DECRYPT
};

static short get_operation_index(Operations operation) //TODO: remove?
{
    switch (operation) {
        case ENCRYPT:
            return 1;
        case DECRYPT:
            return 2;
        default:
            return -1;
    }
}

static constexpr short get_operation_index2(Operations operation)
{
    return static_cast<std::underlying_type_t<Operations>>(operation);
}

static const std::map<Operations, std::string> enums_names = {
        {Operations::ENCRYPT, "Encrypt"},
        {Operations::DECRYPT, "Decrypt"}
};


#endif //AES_OPERATIONS_H
