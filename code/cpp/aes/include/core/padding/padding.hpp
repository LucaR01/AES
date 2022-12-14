//
// Created by Luca on 09/12/2022.
//

#ifndef AES_PADDING_H
#define AES_PADDING_H

#include <map>
#include <string_view>

//TODO: rename directory in Paddings?

namespace aes::pad {

enum class Paddings {
    NO_PADDING = 1,
    PKCS5,
};

//TODO: static constexpr?
unsigned short get_padding_index(Paddings padding);

static const std::map<Paddings, std::string_view> padding_names = {
        {Paddings::NO_PADDING, "No padding"},
        {Paddings::PKCS5, "PKCS5"}
};

} // aes::pad

#endif //AES_PADDING_H
