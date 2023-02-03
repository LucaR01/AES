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
#include "core/padding/padding.hpp"

namespace aes::mod {

enum class Modes {
    ECB = 1,
    CBC,
    CTR
};

static constexpr uint8_t NUM_OF_MODES = 3;

static constexpr std::array<Modes, NUM_OF_MODES> all = { Modes::ECB, Modes::CBC, Modes::CTR };

//TODO: ritornare un byte? uint8_t? char?
[[nodiscard]] static constexpr uint8_t get_mode_index(const Modes& mode)
{
    return static_cast<std::underlying_type_t<Modes>>(mode);
}

static const std::map<Modes, std::string_view> modes_names = {
        {Modes::ECB, "ECB"},
        {Modes::CBC, "CBC"},
        {Modes::CTR, "CTR"}
};

//TODO: da chiamare nella console.
//TODO: metterlo in aes.hpp?
std::vector<uint8_t> encrypt(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const pad::Paddings& padding, const mod::Modes& mode);

//TODO: uncomment
//std::string_view encrypt2(std::string& message, std::string& key, const std::optional<std::vector<uint8_t>>& iv, const pad::Paddings& padding, const mod::Modes& mode);

//TODO: remove
//int encrypt_ECB(const std::vector<uint8_t>& key, const AES& aes, std::vector<std::array<std::array<uint8_t, gal::BLOCK_WORDS>, gal::BLOCK_WORDS>>& keys);

// ---------------------------------------------------------------------------------------------------------------------

uint8_t* encrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

uint8_t* decrypt_ECB(const uint8_t input[], const unsigned int& input_length, const uint8_t key[], const aes::AES& aes);

std::vector<uint8_t> encrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes); //TODO: uncomment both.

std::vector<uint8_t> decrypt_ECB(const std::vector<uint8_t>& input, const std::vector<uint8_t>& key, const AES& aes);

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

} // aes::mod

#endif //AES_MODES_H
