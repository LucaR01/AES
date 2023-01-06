//
// Created by Luca on 06/01/2023.
//

#ifndef AES_PASSWORD_H
#define AES_PASSWORD_H

#include <string>

namespace aes::pwd {

//TODO: rename in Key?
class Password {
public:
    Password() = delete; //TODO: oppure creare un generatore casuale di password.
    Password(std::string_view& password);
    ~Password() = default;

    [[nodiscard]] std::string_view get_password() const;

private:
    std::string raw_password;
    std::string_view salted_and_pepper_password;
    std::string_view salt;
    static constexpr std::string_view PEPPER = "$"; //TODO: update
    static constexpr std::string_view SALT = ""; //TODO: il salt dovrebbe essere casuale! e mantenuto in un database.

    std::string_view generate_random_salt();
    void hash_password();
};

} // namespace aes::pwd

#endif //AES_PASSWORD_H
