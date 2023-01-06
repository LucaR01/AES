//
// Created by Luca on 06/01/2023.
//

#include "password/password.hpp"

namespace aes::pwd {

/*Password::Password() //TODO: remove or uncomment.
{

}*/

Password::Password(std::string_view &password)
{
    this->salted_and_pepper_password = this->raw_password.append(Password::SALT).append(Password::PEPPER);
    //this->salted_and_pepper_password = this->raw_password.append(generate_random_salt()).append(Password::PEPPER); //TODO: uncomment
}

[[nodiscard]] std::string_view Password::get_password() const
{
    return this->raw_password;
}

std::string_view Password::generate_random_salt()
{
    //TODO:
    return "";
}

void Password::hash_password()
{
    //TODO:
}

} // namespace aes::pwd
