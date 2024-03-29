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

enum class Operations { //TODO: rename in ENCRYPTION e DECRYPTION?
    ENCRYPT = 1,
    DECRYPT
};

static constexpr uint8_t NUM_OF_OPERATIONS = 2;

static constexpr std::array<Operations, NUM_OF_OPERATIONS> ALL_OPERATIONS{Operations::ENCRYPT, Operations::DECRYPT};

static const std::map<Operations, std::string_view>& OPERATIONS_NAMES = {
        {Operations::ENCRYPT, "Encrypt"},
        {Operations::DECRYPT, "Decrypt"}
};

//TODO: rinominare in qualcos'altro? EncryptionObjects?
//TODO: rinominare in InputOperations?
enum class EncryptionOperations {
    MESSAGE = 1,
    FILE
};

static constexpr uint8_t NUM_OF_ENCRYPTION_OPERATIONS = 2;

static constexpr std::array<EncryptionOperations, NUM_OF_ENCRYPTION_OPERATIONS> ALL_ENCRYPTION_OPERATIONS{EncryptionOperations::MESSAGE, EncryptionOperations::FILE};

//TODO: touppercase because it's a static constexpr
static const std::map<EncryptionOperations, std::string_view>& ENCRYPTION_OPERATIONS_NAMES = {
        {EncryptionOperations::MESSAGE, "Message"},
        {EncryptionOperations::FILE, "File"}
};

/**
 * @brief: This function allows to retrieve the index of an element of an enum.
 * @param op : You pass either an element of the @enum Operations or of the @enum EncryptionOperations.
 * @return : the index of the element in the respective enum.
 */
[[nodiscard]] static constexpr uint8_t get_operation_index(const std::variant<Operations, EncryptionOperations>& op)
{
    if(std::holds_alternative<Operations>(op)) {
        return static_cast<std::underlying_type_t<Operations>>(std::get<Operations>(op));
    } else {
        return static_cast<std::underlying_type_t<Operations>>(std::get<EncryptionOperations>(op));
    }
}

} //namespace aes::ops

#endif //AES_OPERATIONS_H
