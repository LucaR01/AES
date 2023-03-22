//
// Created by Luca on 01/03/2023.
//

#ifndef AES_VERSION_HPP
#define AES_VERSION_HPP

#include <string>
#include <string_view>

//TODO: oppure aes::ver?
namespace aes::vrs {

/**
 * This struct stores the software's version.
 */
struct AES {
    static constexpr unsigned short MAJOR = 1;
    static constexpr unsigned short MINOR = 0;
    static constexpr unsigned short PATCH = 0;
    static constexpr std::string_view NAME = "AES"; //TODO: rename in software_name?
    const std::string version = std::string(NAME) + " v" + std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH); //TODO: string_view
};

static const AES aes{};

} // namespace aes::vrs

#endif //AES_VERSION_HPP
