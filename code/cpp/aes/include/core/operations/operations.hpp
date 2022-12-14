//
// Created by Luca on 11/12/2022.
//

#ifndef AES_OPERATIONS_H
#define AES_OPERATIONS_H

#include <type_traits>
#include <map>
#include <string_view>

namespace aes::ops {

//TODO: rename in EncryptionOperations?
enum Operations { //TODO: rename in ENCRYPTION e DECRYPTION?
    ENCRYPT = 1,
    DECRYPT
};

//TODO: static constexpr
unsigned short get_operation_index(Operations operation);

static const std::map<Operations, std::string_view> enums_names = {
        {Operations::ENCRYPT, "Encrypt"},
        {Operations::DECRYPT, "Decrypt"}
};

//TODO: mettere anche il resto in questo namespace?
// È in un namespace, perché mi dava errore di ridefinizione di FILE.

//TODO: rinominare in qualcos'altro? EncryptionObjects?
enum EncryptionOperations {
    MESSAGE = 1,
    FILE
};

static const std::map<EncryptionOperations, std::string_view> encryption_operations_names = {
        {EncryptionOperations::MESSAGE, "Message"},
        {EncryptionOperations::FILE, "File"}
};

} //namespace aes::ops


#endif //AES_OPERATIONS_H
