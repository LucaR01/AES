//
// Created by Luca on 14/12/2022.
//

#include "core/padding/padding.hpp"
#include "core/aes.hpp"
#include "math/random/aes_random.hpp"

#include "logger/logger.hpp"

namespace aes::pad {

[[maybe_unused]] std::string add_padding(std::string& message, const Paddings& padding)
{
    const unsigned int& remainder = message.size() % aes::BLOCK_SIZE;
    const unsigned int& missing_length = aes::BLOCK_SIZE - remainder;

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
                message.insert(message.size(), aes::BLOCK_SIZE - 1, aes::rnd::get_random_byte());
                message.append(std::to_string(aes::BLOCK_SIZE));
                break;
            case Paddings::NO_PADDING:
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
                message.insert(message.size(), missing_length, '0');
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
                message.insert(message.size(), missing_length - 1, aes::rnd::get_random_byte());
                message.append(std::to_string(missing_length));
                break;
            // Aggiungiamo come padding il numero totale di bytes
            case Paddings::PKCS7:
                AES_INFO("PKCS7 SELECTED")
                for(unsigned int i = 0; i < missing_length; i++) {
                    message.append(std::to_string(missing_length));
                }
                break;
            default:
            case Paddings::NO_PADDING:
                AES_INFO("NO_PADDING SELECTED")
                break;
        }
    }

    return message;
}

std::vector<uint8_t> add_padding(std::vector<uint8_t>& message, const Paddings& padding)
{
    const unsigned int& remainder = message.size() % aes::BLOCK_SIZE;
    const unsigned int& missing_length = aes::BLOCK_SIZE - remainder;

    if(remainder == 0) {
        switch(padding) {
            // Se il messaggio ha la grandezza del blocco (ovvero 16) allora aggiungiamo un altro blocco formato da il numero 1 iniziale + 15 zeri (16 - 1)
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                message.push_back('1');
                message.insert(message.cend(), aes::BLOCK_SIZE - 1, '0');
                break;
                // Se il messaggio ha la grandezza del blocco (ovvero 16) o i suoi multipli allora aggiungiamo un altro blocco composto da 15 bytes casuali e
                // nell'ultimo byte inseriamo il numero totale di bytes aggiunti (ovvero 16).
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                message.insert(message.cend(), aes::BLOCK_SIZE - 1, aes::rnd::get_random_byte());
                message.push_back(aes::BLOCK_SIZE);
                break;
            case Paddings::NO_PADDING:
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
                message.insert(message.cend(), missing_length, '0');
                break;
                // Aggiungiamo un 1 all'inizio del padding e poi tanti zeri quanto missing_length - 1 (quel -1 perché inseriamo quel 1 iniziale)
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                message.push_back('1');
                message.insert(message.cend(), missing_length - 1, '0');
                break;
                // Aggiungiamo tanti zeri e nell'ultimo byte mettiamo il numero totale di bytes aggiunti come singolo valore.
            case Paddings::ANSI_X9_23_PADDING:
                AES_INFO("ANSI X9.23 PADDING SELECTED")
                message.insert(message.cend(), missing_length - 1, '0');
                message.push_back(missing_length);
                break;
                // Aggiungiamo dei bytes casuali, tranne nell'ultimo byte in cui inseriamo la somma totale di bytes aggiunti.
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                message.insert(message.cend(), missing_length - 1, aes::rnd::get_random_byte());
                message.push_back(missing_length); //TODO: questo potrebbe dare errore, perché il valore potrebbe essere superiore a 256 oppure potrebbe non essere
                //TODO: mostrato come valore, ma come carattere.
                break;
                // Aggiungiamo come padding il numero totale di bytes
            case Paddings::PKCS7:
                AES_INFO("PKCS7 SELECTED")
                message.insert(message.cend(), missing_length, missing_length);
                break;
            default:
            case Paddings::NO_PADDING:
                AES_INFO("NO_PADDING SELECTED")
                break;
        }
    }

    AES_DEBUG("message dopo l'aggiunta del padding: {}", std::string(message.cbegin(), message.cend()))

    return message;
}

std::vector<uint8_t> remove_padding(std::vector<uint8_t>& decrypted_message, const Paddings& padding)
{
    const unsigned int& remainder = decrypted_message.size() % aes::BLOCK_SIZE;
    const unsigned int& missing_length = aes::BLOCK_SIZE - remainder;

    if(remainder == 0) {
        switch(padding) {
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                // abbiamo il messaggio che potrebbe essere anche più di 16 e partiamo da 0 con .cbegin() + tutto tranne il blocco aggiunto
                // c.begin() = posizione 0 + (tutto il messaggio - il blocco) = posizione inziale da cui vogliamo cancellare.
                decrypted_message.erase(decrypted_message.cbegin() + static_cast<long long>(decrypted_message.size() - aes::BLOCK_SIZE), decrypted_message.cend());
                break;
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                decrypted_message.erase(decrypted_message.cbegin() + static_cast<long long>(decrypted_message.size() - aes::BLOCK_SIZE), decrypted_message.cend());
                break;
            case Paddings::NO_PADDING:
                AES_INFO("NO_PADDING SELECTED")
                break;
            default:
                AES_INFO("OTHER PADDING THAT DOESN'T REQUIRE PADDING IF THE BLOCK IS FULL = EQUAL TO 16")
                break;
        }
    } else {
        switch(padding) {
            case Paddings::ZERO_PADDING:
                AES_INFO("0-Padding SELECTED")
                // dall'inizio del vettore + numero posizioni del remainder - fino alla fine del vettore -> cancello padding.
                decrypted_message.erase(decrypted_message.cbegin() + remainder, decrypted_message.cend());
                break;
            case Paddings::ONE_ZERO_PADDING:
                AES_INFO("1-0-Padding SELECTED")
                decrypted_message.erase(decrypted_message.cbegin() + remainder, decrypted_message.cend());
                break;
            case Paddings::ANSI_X9_23_PADDING:
                AES_INFO("ANSI X9.23 PADDING SELECTED")
                decrypted_message.erase(decrypted_message.cbegin() + remainder, decrypted_message.cend());
                break;
            case Paddings::ISO_10126_PADDING:
                AES_INFO("ISO 10126 PADDING SELECTED")
                decrypted_message.erase(decrypted_message.cbegin() + remainder, decrypted_message.cend());
                break;
            case Paddings::PKCS7:
                AES_INFO("PKCS7 SELECTED")
                decrypted_message.erase(decrypted_message.cbegin() + remainder, decrypted_message.cend());
                break;
            default:
            case Paddings::NO_PADDING:
                AES_INFO("NO_PADDING SELECTED")
                break;
        }
    }

    return decrypted_message;
}

} // namespace aes::pad
