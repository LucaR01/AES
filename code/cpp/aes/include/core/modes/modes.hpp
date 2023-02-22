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

#include "core/aes.hpp"

namespace aes::mod {

enum class Modes {
    ECB = 1,
    CBC,
    CFB
};

static constexpr uint8_t NUM_OF_MODES = 3;

static constexpr std::array<Modes, NUM_OF_MODES> ALL_MODES = { Modes::ECB, Modes::CBC, Modes::CFB };

[[nodiscard]] static constexpr uint8_t get_mode_index(const Modes& mode)
{
    return static_cast<std::underlying_type_t<Modes>>(mode);
}

static const std::map<Modes, std::string_view>& MODES_NAMES = {
        {Modes::ECB, "ECB"},
        {Modes::CBC, "CBC"},
        {Modes::CFB, "CFB"}
};

//TODO: remove
//int encrypt_ECB(const std::vector<uint8_t>& key, const AES& aes, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys);

// ---------------------------------------------------------------------------------------------------------------------

uint8_t* encrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

uint8_t* decrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO: uncomment both.

std::vector<uint8_t> decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes);

//std::unique_ptr<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const aes::AES& aes); //TODO:

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

uint8_t* encrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

uint8_t* decrypt_CBC(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

std::vector<uint8_t> encrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

std::vector<uint8_t> decrypt_CBC(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

//TODO: fare quelle che restsituiscono std::vector proprio come encrypt_ECB4

// ---------------------------------------------------------------------------------------------------------------------

// CFB MODE

uint8_t* encrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

uint8_t* decrypt_CFB(const uint8_t input[], unsigned int input_length, const uint8_t key[], const uint8_t* iv, const aes::AES& aes);

std::vector<uint8_t> encrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

std::vector<uint8_t> decrypt_CFB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv, const aes::AES& aes);

//TODO: fare quelle che restsituiscono std::vector proprio come encrypt_ECB4

} // aes::mod

#endif //AES_MODES_H
