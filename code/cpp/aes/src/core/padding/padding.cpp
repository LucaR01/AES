//
// Created by Luca on 14/12/2022.
//

#include <type_traits>
#include <random>
#include <algorithm> // per generate
#include <climits> // per CHAR_BIT
#include <functional> // per

#include "core/padding/padding.hpp"
#include "core/aes.hpp"
#include "logger/logger.hpp"

namespace aes::pad {

std::string add_padding(std::string& message, const Paddings& padding)
{
    unsigned int remainder = message.size() % aes::BLOCK_SIZE;
    unsigned int missing_length = aes::BLOCK_SIZE - remainder;

    if(remainder == 0) {
        switch(padding) {
            // Se il messaggio ha la grandezza del blocco (ovvero 16) allora aggiungiamo un altro blocco formato da il numero 1 iniziale + 15 zeri (16 - 1)
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                message.append("1");
                message.insert(message.size(), aes::BLOCK_SIZE - 1, '0');
                break;
            // Se il messaggio ha la grandezza del blocco (ovvero 16) o i suoi multipli allora aggiungiamo un altro blocco composto da 15 bytes casuali e
            // nell'ultimo byte inseriamo il numero totale di bytes aggiunti (ovvero 16).
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                message.insert(message.size(), aes::BLOCK_SIZE - 1, get_random_byte());
                message.append(std::to_string(aes::BLOCK_SIZE));
                break;
            case Paddings::NO_PADDING:
            //case default:
                AES_INFO("NO_PADDING SELECTED")
                break;
            default:
                AES_INFO("OTHER PADDING THAT DOESN'T REQUIRE PADDING IF THE BLOCK IS FULL = EQUAL TO 16")
                break;
        }
    } else {
        switch(padding) {
            // Aggiungiamo 0 fino a riempire la stringa, se la stringa non è un divisore di 16 (che è la grandezza del blocco)
            // Ma se il blocco è già 16, allora non aggiungiamo gli 0.
            case Paddings::ZERO_PADDING:
                AES_INFO("0-Padding SELECTED")
                message.insert(message.size(), missing_length, '0'); //TODO: prima era message.back() ma non andava
                break;
            // Aggiungiamo un 1 all'inizio del padding e poi tanti zeri quanto missing_length - 1 (quel -1 perché inseriamo quel 1 iniziale)
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                message.append("1");
                message.insert(message.size(), missing_length - 1, '0');
                break;
            // Aggiungiamo tanti zeri e nell'ultimo byte mettiamo il numero totale di bytes aggiunti come singolo valore.
            case Paddings::ANSI_X9_23_PADDING:
                AES_INFO("ANSI X9.23 PADDING SELECTED")
                message.insert(message.size(), missing_length - 1, '0');
                message.append(std::to_string(missing_length));
                break;
            // Aggiungiamo dei bytes casuali, tranne nell'ultimo byte in cui inseriamo la somma totale di bytes aggiunti.
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                message.insert(message.size(), missing_length - 1, get_random_byte());
                message.append(std::to_string(missing_length));
                break;
            // Aggiungiamo come padding il numero totale di bytes
            case Paddings::PKCS7:
                AES_INFO("PKCS7 SELECTED")
                //message.insert(message.size(), missing_length, missing_length); //TODO: remove
                for(unsigned int i = 0; i < missing_length; i++) {
                    message.append(std::to_string(missing_length));
                }
                break;
            case Paddings::NO_PADDING:
            //case default:
                AES_INFO("NO_PADDING SELECTED")
                break;
        }
    }

    //return std::vector<uint8_t>(message, message.size() + 0);
    return message;
}

//TODO: spostare in un altro file tipo Random.hpp

unsigned int get_seed()
{
    std::random_device random_device;
    std::mt19937 random_device_generator(random_device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution{};
    return distribution(random_device_generator);
}

//TODO: questa è da rifattorizzare
char get_random_byte()
{
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t> random_byte_engine(get_seed()); //TODO: prima srand(time(NULL)) ma non andava.
    std::vector<char> data(1); // 1 perché ci serve un singolo byte, quindi un singolo carattere.
    std::generate(data.begin(), data.end(), std::ref(random_byte_engine));
    return data.at(0);
}

std::vector<char> get_random_bytes(const unsigned int& number_of_random_bytes)
{
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t> random_byte_engine(get_seed());
    //std::array<uint8_t, number_of_random_bytes> data{}; //TODO: questo non è possibile.
    std::vector<char> data(number_of_random_bytes);
    std::generate(data.begin(), data.end(), std::ref(random_byte_engine));
    return data;
}

} // namespace aes::pad
