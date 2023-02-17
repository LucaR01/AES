//
// Created by Luca on 09/12/2022.
//

#ifndef AES_PADDING_H
#define AES_PADDING_H

#include <map>
#include <string_view>
#include <array>
#include <cstdint>
#include <vector>
#include <string>

//TODO: rename directory in paddings?
//TODO: rename file in paddings?

namespace aes::pad {

enum class Paddings {
    NO_PADDING = 1,
    ZERO_PADDING,
    ONE_ZERO_PADDING,
    ANSI_X9_23_PADDING,
    ISO_10126_PADDING,
    PKCS7
};

static constexpr uint8_t TYPES_OF_PADDINGS = 6;

//TODO: rename in ALL_PADDING_TYPES?
static constexpr std::array<Paddings, TYPES_OF_PADDINGS> ALL_PADDINGS{Paddings::NO_PADDING, Paddings::ZERO_PADDING, Paddings::ONE_ZERO_PADDING,
                                                             Paddings::ANSI_X9_23_PADDING, Paddings::ISO_10126_PADDING, Paddings::PKCS7};

[[nodiscard]] static constexpr uint8_t get_padding_index(const Paddings& padding)
{
    return static_cast<std::underlying_type_t<Paddings>>(padding);
}

//TODO: rename in PADDINGS_NAMES?
static const std::map<Paddings, std::string_view>& PADDING_NAMES = {
        {Paddings::NO_PADDING, "No Padding"},
        {Paddings::ZERO_PADDING, "0-Padding"},
        {Paddings::ONE_ZERO_PADDING, "1-0-Padding"},
        {Paddings::ANSI_X9_23_PADDING, "ANSI x9.23 Padding"},
        {Paddings::ISO_10126_PADDING, "ISO 10126 Padding"},
        {Paddings::PKCS7, "PKCS7"}
};

//TODO: volendo string_view
[[maybe_unused]] std::string add_padding(std::string& message, const Paddings& padding);

std::vector<uint8_t> add_padding(std::vector<uint8_t>& message, const Paddings& padding);

//TODO: rename in remove_padding_from_message()?
std::vector<uint8_t> remove_padding(std::vector<uint8_t>& encrypted_message, const Paddings& padding);

} // aes::pad

#endif //AES_PADDING_H
