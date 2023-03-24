//
// Created by Luca on 09/12/2022.
//

#include <iostream>
#include <string>

#include "graphics/console/console.hpp"
#include "logger/logger.hpp"
#include "file_manager/file_manager.hpp"

#include "core/aes_api.hpp"

namespace aes::con {

void show_console(const std::pair<aes::AES, arg::Arguments>& aes_argument, const std::pair<aes::mod::Modes, arg::Arguments>& mode_argument, const std::pair<aes::pad::Paddings, arg::Arguments>& padding_argument, const std::pair<std::string, arg::Arguments>& message_argument,
                  const std::pair<std::string, arg::Arguments>& key_argument, const std::pair<std::string, aes::arg::Arguments>& iv_argument, const std::pair<std::string, aes::arg::Arguments>& input_file_path_argument,
                  const std::pair<std::string, arg::Arguments>& output_file_path_argument, const std::pair<aes::ops::Operations, arg::Arguments>& operation_argument)
{
    const aes::ops::Operations& operation = operation_argument.second == aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_operation() : operation_argument.first;
    const aes::ops::EncryptionOperations& encryption_operation = request_file_or_message_operation();
    std::string message = encryption_operation == ops::EncryptionOperations::MESSAGE ? message_argument.second == arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_message() : message_argument.first : std::string(def::EMPTY_STRING);
    const AES& aes = aes_argument.second == arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_aes_type() : aes_argument.first;
    const aes::mod::Modes& mode = mode_argument.second == arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_mode() : mode_argument.first;
    const aes::pad::Paddings& padding = padding_argument.second == arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_padding() : padding_argument.first;
    std::string key = key_argument.second == aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_key() : key_argument.first;
    const std::string& input_file_path = encryption_operation == ops::EncryptionOperations::FILE ? input_file_path_argument.second == aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_input_file() : input_file_path_argument.first : std::string(def::EMPTY_STRING);
    const std::string& output_file_path = encryption_operation == ops::EncryptionOperations::FILE ? output_file_path_argument.second == aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_output_file() : output_file_path_argument.first : std::string(def::EMPTY_STRING);
    std::vector<uint8_t> iv;

    if(mode != aes::mod::Modes::ECB) {
        iv = iv_argument.second == aes::arg::Arguments::NOT_USER_PASSED_ARGUMENT ? request_iv(mode) : std::vector<uint8_t>(iv_argument.first.cbegin(), iv_argument.first.cend());
    }

    AES_DEBUG("console | operation: {}", ops::OPERATIONS_NAMES.at(operation))
    AES_DEBUG("console | encryption_operation: {}", ops::ENCRYPTION_OPERATIONS_NAMES.at(encryption_operation))
    AES_DEBUG("console | message: {}", message)
    AES_DEBUG("console | aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))
    AES_DEBUG("console | mode: {}", mod::MODES_NAMES.at(mode))
    AES_DEBUG("console | padding: {}", pad::PADDING_NAMES.at(padding))
    AES_DEBUG("console | key: {}", key)
    AES_DEBUG("console | input_file_path: {}", input_file_path)
    AES_DEBUG("console | output_file_path: {}", output_file_path)
    AES_DEBUG("console | iv: {}", std::string(iv.cbegin(), iv.cend()))

    encrypt_decrypt_message_or_file(encryption_operation, operation, message, key, aes, mode, padding, iv, input_file_path, output_file_path);
}

//TODO: però questa la utilizzo una singola volta, la tengo lo stesso? Per il principio di Single Responsability Principle.
aes::ops::Operations request_operation()
{
    unsigned short operation_selected; //TODO: renderlo un Operations di default?

    //TODO: volendo aggiungere la possibilità di avere più lingue.
    std::cout << "Seleziona l'operazione:" << '\n';
    for(const auto& op : aes::ops::ALL_OPERATIONS) {
        std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(op)) << ". " << aes::ops::OPERATIONS_NAMES.at(op) << '\n';
    }
    std::cout << "[Input: 1|2] Scelta: ";

    std::cin >> operation_selected;
    AES_TRACE("operation_selected: {}", operation_selected)
    //TODO: cambiare la condizione nel while.
    while((std::cin.fail()) || (operation_selected < aes::ops::get_operation_index(aes::ops::ALL_OPERATIONS.front()) || operation_selected > aes::ops::ALL_OPERATIONS.size())) {
        //std::cout << "[Input: 1|2] Scelta: "; //TODO: uncomment se lascio solo questo al posto di quello sotto.
        for(const auto& op : aes::ops::ALL_OPERATIONS) { //TODO: forse è quasi meglio non ristamparli. (non serve, sono già sopra)
            std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(op)) << ". " << aes::ops::OPERATIONS_NAMES.at(op) << '\n';
        }
        std::cout << "[Input: 1|2] Scelta: ";
        std::cin >> operation_selected;
        std::cout << std::flush;
        AES_TRACE("mode_selected: {}", operation_selected)
    }

    std::cin.clear(); //TODO: remove?

    aes::ops::Operations op = static_cast<aes::ops::Operations>(operation_selected);
    std::cout << "Operazione selezionata: " << aes::ops::OPERATIONS_NAMES.at(op) << std::endl;

    return op;
}

// ENCRYPTION & DECRYPTION

//TODO: optional iv?
void encrypt_decrypt_message_or_file(const aes::ops::EncryptionOperations& encryption_operation, const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding,
                             const std::vector<uint8_t>& iv, const std::string& input_file_path, const std::string& output_file_path)
{
    switch(encryption_operation) {
        case aes::ops::EncryptionOperations::MESSAGE:
            AES_INFO("Inside MESSAGE case, encryption_operation: {}", aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(encryption_operation))
            encrypt_decrypt_message(operation, message, key, aes, mode, padding, iv);
            break;
        case aes::ops::EncryptionOperations::FILE:
            AES_INFO("Inside FILE case, encryption_operation: {}", aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(encryption_operation))
            encrypt_decrypt_file(operation, aes::fm::FileManager::get_file_data3(input_file_path), output_file_path, key, aes, mode, padding, iv);
            break;
        default:
            AES_CRITICAL("encryption_decryption_operation should not be in default case, encryption_operation: {}", aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(encryption_operation))
            exit(EXIT_FAILURE);
    }
}

void encrypt_decrypt_message(const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv)
{
    switch(operation) {
        case ops::Operations::ENCRYPT:
        {
            const std::vector<uint8_t>& ciphertext = aes::api::encrypt(message, key, iv, padding, mode, aes);
            //TODO: usare aes::cvt::get_string_from_vector()
            std::cout << "Messaggio cifrato: " << std::string(ciphertext.cbegin(), ciphertext.cend()) << std::endl;
            break;
        }
        case ops::Operations::DECRYPT:
        {
            const std::vector<uint8_t>& deciphered_plaintext = aes::api::decrypt(message, key, iv, padding, mode, aes);
            //TODO: usare aes::cvt::get_string_from_vector()
            std::cout << "Messaggio decifrato: " << std::string(deciphered_plaintext.cbegin(), deciphered_plaintext.cend()) << std::endl;
            break;
        }
    }
}

void encrypt_decrypt_file(const aes::ops::Operations& operation, std::string input_file_data, const std::string& output_file_path, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv)
{
    //TODO: operation == ops::Operations::ENCRYPT ? aes::api::encrypt_file(input_file_data, output_file_path, key, iv, padding, mode, aes) : aes::api::decrypt_file(input_file_data, output_file_path, key, iv, padding, mode, aes);
    switch(operation) {
        case ops::Operations::ENCRYPT:
        {
            aes::api::encrypt_file(input_file_data, output_file_path, key, iv, padding, mode, aes);
            break;
        }
        case ops::Operations::DECRYPT:
        {
            aes::api::decrypt_file(input_file_data, output_file_path, key, iv, padding, mode, aes);
            break;
        }
    }
}


// REQUEST

std::string request_message()
{
    std::string message;
    std::cout << "Inserire messaggio: ";
    std::cin.ignore();
    std::getline(std::cin, message);
    AES_DEBUG("Message from user input: {}", message)

    return message;
}

std::string request_input_file()
{
    std::string file_path;
    std::cout << "Inserire path del file di input: ";
    std::cin.ignore();
    std::getline(std::cin, file_path);
    AES_DEBUG("Input File path from user input: {}", file_path)

    return file_path;
}

std::string request_output_file()
{
    std::string file_path;
    std::cout << "Inserire path del file di output: ";
    std::cin.ignore();
    std::getline(std::cin, file_path);
    AES_DEBUG("Output File path from user input: {}", file_path)

    return file_path;
}

aes::pad::Paddings request_padding()
{
    unsigned short padding_type;
    std::cout << "Seleziona tipo di padding: " << '\n';
    for(const auto& p : aes::pad::ALL_PADDINGS) {
        std::cout << static_cast<unsigned short>(aes::pad::get_padding_index(p)) << ". " << aes::pad::PADDING_NAMES.at(p) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> padding_type;

    while((std::cin.fail()) || (padding_type < aes::pad::get_padding_index(aes::pad::ALL_PADDINGS.front()) || padding_type > aes::pad::get_padding_index(aes::pad::ALL_PADDINGS.back()))) {
        std::cin.clear();
        std::cout << "Seleziona tipo di padding: " << '\n';
        for(const auto& p : aes::pad::ALL_PADDINGS) {
            std::cout << static_cast<unsigned short>(aes::pad::get_padding_index(p)) << ". " << aes::pad::PADDING_NAMES.at(p) << '\n';
        }
        std::cout << "Seleziona: " << std::flush;
        std::cin >> padding_type;
        AES_DEBUG("padding_type: {}", padding_type)
    }

    aes::pad::Paddings selected_padding_type = static_cast<aes::pad::Paddings>(padding_type);
    std::cout << "Hai selezionato: " << aes::pad::PADDING_NAMES.at(selected_padding_type) << std::endl;
    return selected_padding_type;
}

std::string request_key()
{
    unsigned short user_input_operation;
    std::cout << "Inserire chiave da input o da file?" << '\n'; //TODO: riscrivere?
    for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
        std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
    }

    std::cout << "Seleziona: ";
    std::cin >> user_input_operation;
    //TODO: < ops::get_operation_index(ops::ALL_ENCRYPTION_OPERATIONS.front()) || user_input_operation > ops::ALL_ENCRYPTION_OPERATIONS.size()); prima era < 1 || > 2
    while((std::cin.fail()) || user_input_operation < ops::get_operation_index(ops::ALL_ENCRYPTION_OPERATIONS.front()) || user_input_operation > ops::ALL_ENCRYPTION_OPERATIONS.size()) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << "Inserire chiave da input o da file?" << '\n';
        for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
            std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> user_input_operation;
        std::cout << std::flush;
        AES_DEBUG("user_input_operation: {}", user_input_operation)
    }

    std::cin.clear();

    auto input_operation = static_cast<ops::EncryptionOperations>(user_input_operation);

    std::string key;

    switch(input_operation) {
        case ops::EncryptionOperations::MESSAGE:
            std::cout << "Inserire chiave: ";
            std::cin.ignore();
            std::getline(std::cin, key);
            AES_DEBUG("Key from user input: {}", key)
            break;
        case ops::EncryptionOperations::FILE:
            std::string file_path;
            std::cout << "Inserire path del file: ";
            std::cin.ignore();
            std::getline(std::cin, file_path);
            key = aes::fm::FileManager::get_key(file_path);
            break;
    }

    return key;
}

aes::mod::Modes request_mode()
{
    unsigned short mode;
    std::cout << "Seleziona modalità: " << '\n';
    for(const auto& m : aes::mod::ALL_MODES) {
        std::cout << static_cast<unsigned short>(aes::mod::get_mode_index(m)) << ". " << aes::mod::MODES_NAMES.at(m) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> mode;

    while((std::cin.fail()) || (mode < aes::mod::get_mode_index(mod::ALL_MODES.front()) || mode > aes::mod::get_mode_index(mod::ALL_MODES.back()))) {
        std::cin.clear();
        for(const auto& e : aes::mod::ALL_MODES) {
            //TODO: if mode == ECB, stampare anche [deprecated] affianco alla scelta.
            std::cout << static_cast<unsigned short>(aes::mod::get_mode_index(e)) << ". " << aes::mod::MODES_NAMES.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> mode;
        AES_DEBUG("mode: {}", mode)
    }

    aes::mod::Modes selected_mode = static_cast<aes::mod::Modes>(mode);
    std::cout << "Hai selezionato: " << aes::mod::MODES_NAMES.at(selected_mode) << std::endl;
    return selected_mode;
}

std::vector<uint8_t> request_iv(const aes::mod::Modes& mode)
{
    if(mode == aes::mod::Modes::ECB) {
        return {}; //TODO: restituire nel migliore modo per restituire un empty optional; prima era return {}; prima era std::nullopt_t
    }

    std::string iv;
    std::cout << "Inserire iv: ";
    std::cin.ignore();
    std::getline(std::cin, iv);

    AES_DEBUG("IV from user input: {}", iv)

    return {iv.cbegin(), iv.cend()};
}

aes::AES request_aes_type()
{
    unsigned short aes_type;
    std::cout << "Selezionare tipologia AES: " << '\n';
    for(const auto& t : aes::ALL_AES_TYPES) {
        std::cout << aes::get_aes_index(t) << ". " << aes::ALL_AES_TYPES_NAMES.at(t) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> aes_type;
    while((std::cin.fail()) || !aes::AES_VALUES.contains(aes_type)) {
        std::cin.clear();
        for(const auto& t : aes::ALL_AES_TYPES) {
            std::cout << aes::get_aes_index(t) << ". " << aes::ALL_AES_TYPES_NAMES.at(t) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> aes_type;
        std::cout << std::flush;
        AES_DEBUG("aes_type: {}", aes_type)
    }

    std::cin.clear();

    return static_cast<aes::AES>(aes_type);
}

aes::ops::EncryptionOperations request_file_or_message_operation()
{
    unsigned short file_or_message_operation;
    std::cout << "Su cosa si desidera eseguire questa operazione?" << '\n';
    for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
        std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> file_or_message_operation;
    while((std::cin.fail()) || (file_or_message_operation < aes::ops::get_operation_index(aes::ops::EncryptionOperations::MESSAGE) || file_or_message_operation > aes::ops::get_operation_index(aes::ops::EncryptionOperations::FILE))) {
        std::cin.clear();
        std::cout << "Su cosa si desidera eseguire questa operazione?" << '\n';
        for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
            std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> file_or_message_operation;
        std::cout << std::flush;
        AES_DEBUG("file_or_message_operation: {}", file_or_message_operation)
    }

    std::cin.clear();

    const aes::ops::EncryptionOperations& file_or_message_operation_selected = static_cast<aes::ops::EncryptionOperations>(file_or_message_operation);
    std::cout << "Operazione selezionata: " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(file_or_message_operation_selected) << std::endl;

    return file_or_message_operation_selected;
}

} //namespace aes::con
