//
// Created by Luca on 11/12/2022.
//

#ifndef AES_OPERATIONS_H
#define AES_OPERATIONS_H

#include <type_traits>
#include <map>
#include <string_view>
#include <array>
#include <variant>

namespace aes::ops {

//TODO: rename in EncryptionOperations?
enum Operations { //TODO: rename in ENCRYPTION e DECRYPTION?
    ENCRYPT = 1,
    DECRYPT
};

static constexpr uint8_t NUM_OF_OPERATIONS = 2; //TODO: rimettere short?

static constexpr std::array<Operations, NUM_OF_OPERATIONS> all_operations{Operations::ENCRYPT, Operations::DECRYPT};

static const std::map<Operations, std::string_view> operations_names = {
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

static constexpr uint8_t NUM_OF_ENCRYPTION_OPERATIONS = 2;

static constexpr std::array<EncryptionOperations, NUM_OF_ENCRYPTION_OPERATIONS> all_encryption_operations{EncryptionOperations::MESSAGE, EncryptionOperations::FILE};

static const std::map<EncryptionOperations, std::string_view> encryption_operations_names = {
        {EncryptionOperations::MESSAGE, "Message"},
        {EncryptionOperations::FILE, "File"}
};

unsigned short get_operation_index(const std::variant<Operations, EncryptionOperations>& op);

} //namespace aes::ops


#endif //AES_OPERATIONS_H
