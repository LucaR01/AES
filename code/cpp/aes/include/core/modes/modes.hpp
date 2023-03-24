//
// Created by Luca on 09/12/2022.
//

#ifndef AES_MODES_H
#define AES_MODES_H

#include <map>
#include <string_view>
#include <array>
#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "core/aes.hpp"

namespace aes::mod {

enum class Modes {
    ECB = 1,
    CBC,
    CFB
};

static constexpr uint8_t NUM_OF_MODES = 3;

static constexpr std::array<Modes, NUM_OF_MODES> ALL_MODES = { Modes::ECB, Modes::CBC, Modes::CFB };

/**
 * @brief: This function allows to retrieve the index of an element of an enum.
 * @param mode : it's @enum Modes.
 * @return the index of the element in the enum.
 */
[[nodiscard]] static constexpr uint8_t get_mode_index(const Modes& mode)
{
    return static_cast<std::underlying_type_t<Modes>>(mode);
}

static const std::map<Modes, std::string_view>& MODES_NAMES = {
        {Modes::ECB, "ECB"},
        {Modes::CBC, "CBC"},
        {Modes::CFB, "CFB"}
};

/**
 * @brief: This function encrypts the input using the ECB (Electronic CodeBook) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the plaintext that has to be encrypted.
 * @param input_length : It's the length of the #input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @return : it returns a pointer of uint8_t with the encrypted text.
 */
uint8_t* encrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

/**
 * @brief: This function decrypts the input using the ECB (Electronic CodeBook) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the encrypted text that has to be decrypted.
 * @param input_length : It's the length of the \p input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @return : it returns a pointer of uint8_t with the decrypted text.
 */
uint8_t* decrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: uncomment both.

std::vector<uint8_t> decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes);

/**
 *
 * @brief: This function checks whether the parameter @p size is divisible by 16.
 * @param size : it's the size of the length of the input in @fn uint8_t* encrypt_ECB or @fn uint8_t* decrypt_ECB
 */
void verify_length(const unsigned int& size);

// ---------------------------------------------------------------------------------------------------------------------

/*std::vector<uint8_t> encrypt_ECB2(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes);

std::vector<uint8_t> decrypt_ECB2(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes);*/

/*std::shared_ptr<uint8_t> encrypt_ECB3(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes); //TODO: uncomment and retry.

std::shared_ptr<uint8_t> decrypt_ECB3(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

std::vector<uint8_t> encrypt_ECB3(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes);

std::vector<uint8_t> decrypt_ECB3(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes);*/

//uint8_t* encrypt_ECB(const uint8_t input[], const uint8_t key[], const aes::AES& aes);

//uint8_t* decrypt_ECB(const uint8_t input[], const uint8_t key[], const aes::AES& aes);

uint8_t* encrypt_ECB4(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: funziona, ma è da fattorizzare.

uint8_t* decrypt_ECB4(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: funziona, ma è da fattorizzare.

std::vector<uint8_t> encrypt_ECB5(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: non va

std::vector<uint8_t> decrypt_ECB5(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: non va

// ---------------------------------------------------------------------------------------------------------------------

// CBC MODE

/**
 * @brief: This function encrypts the input using the CBC (Cipher Block chaining) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the plaintext that has to be encrypted.
 * @param input_length : It's the length of the #input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @param iv: It's the initialization vector which is used to provide an initial state. Typically required to be random, pseudorandom, unpredictable or unique.
 * @return : it returns a pointer of uint8_t with the encrypted text.
 */
uint8_t* encrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

/**
 * @brief This function decrypts the input using the CBC (Cipher Block chaining) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the encrypted text that has to be decrypted.
 * @param input_length : It's the length of the input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @param iv: It's the initialization vector which is used to provide an initial state. Typically required to be random, pseudorandom, unpredictable or unique.
 * @return it returns a pointer of uint8_t with the decrypted plaintext.
 */
uint8_t* decrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

std::vector<uint8_t> encrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

std::vector<uint8_t> decrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

//TODO: fare quelle che restsituiscono std::vector proprio come encrypt_ECB4

// ---------------------------------------------------------------------------------------------------------------------

// CFB MODE

/**
 * @brief: This function encrypts the input using the CFB (Cipher Feedback) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the plaintext that has to be encrypted.
 * @param input_length : It's the length of the #input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @param iv: It's the initialization vector which is used to provide an initial state. Typically required to be random, pseudorandom, unpredictable or unique.
 * @return : it returns a pointer of uint8_t with the encrypted text.
 */
uint8_t* encrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

/**
 * @brief: This function decrypts the input using the CFB (Cipher Feedback) mode.
 * @param input : It's an array of uint8_t aka unsigned char. It's the encrypted text that has to be decrypted.
 * @param input_length : It's the length of the #input.
 * @param key : It's an array of uint8_t which is a typedef of unsigned char.
 * @param aes : It's an enum which represents which type of AES to use: 128, 192 or 256.
 * @param iv: It's the initialization vector which is used to provide an initial state. Typically required to be random, pseudorandom, unpredictable or unique.
 * @return : it returns a pointer of uint8_t with the decrypted plaintext.
 */
uint8_t* decrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

std::vector<uint8_t> encrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

std::vector<uint8_t> decrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

//TODO: fare quelle che restsituiscono std::vector proprio come encrypt_ECB4

} // aes::mod

#endif //AES_MODES_H
