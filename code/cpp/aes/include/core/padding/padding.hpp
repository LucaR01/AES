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
#include <type_traits>

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

static constexpr std::array<Paddings, TYPES_OF_PADDINGS> ALL_PADDINGS{Paddings::NO_PADDING, Paddings::ZERO_PADDING, Paddings::ONE_ZERO_PADDING,
                                                             Paddings::ANSI_X9_23_PADDING, Paddings::ISO_10126_PADDING, Paddings::PKCS7};

/**
 * @brief: This function allows to retrieve the index of an element of the @enum Paddings.
 * @param padding : an element of the @enum Paddings.
 * @return : the index as an uint8_t which is a typedef of unsigned char (you need to cast it to an int type if you want a number).
 */
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
/**
 * @brief: This function allows to add a specific padding from the @enum Paddings to a @param message.
 * @param message : It's a std::string& of the message in which you want to add the padding.
 * @param padding : It's an @enum Padding which determines the type of Padding to apply to the @p message.
 * @return : a std::string with the message with the padding at the end.
 */
[[maybe_unused]] std::string add_padding(std::string& message, const Paddings& padding);

/**
 * @brief: This function allows to add a specific padding from the @enum Paddings to a @param message.
 * @param message : It's a std::string& of the message in which you want to add the padding.
 * @param padding : It's an @enum Padding which determines the type of Padding to apply to the @p message.
 * @return : an std::vector<uint8_t> (uint8_t = unsigned char) of the message with the padding.
 */
std::vector<uint8_t> add_padding(std::vector<uint8_t>& message, const Paddings& padding);

/**
 * @brief: This function removes a specific padding from the @param padding of type @enum Paddings from a @p decrypted_message.
 * @param decrypted_message : the decrypted message which we need to remove the padding at the end.
 * @param padding : the specific type of @enum Paddings that has been applied at encryption, that we now need to remove.
 * @return : an std::vector<uint8_t> (uint8_t = unsigned char) of the message without the padding aka decrypted plaintext.
 */
std::vector<uint8_t> remove_padding(std::vector<uint8_t>& decrypted_message, const Paddings& padding);

} // aes::pad

#endif //AES_PADDING_H
