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

//TODO: usare uint8_t dove possibile.

void show_console(const aes::AES& aes_argument, const aes::mod::Modes& mode_argument, const aes::pad::Paddings& padding_argument, const std::string& message_argument, const std::string& key_argument,
                  const std::string& iv_argument, const std::string& input_file_path_argument, const std::string& output_file_path_argument, const aes::ops::Operations& operation_argument)
{
    //TODO:
    if(operation_argument == aes::def::DEFAULT_OPERATION) {
        //TODO:
    } else {
        //TODO:
    }

    const aes::ops::Operations& operation = (operation_argument == aes::def::DEFAULT_OPERATION) ? request_operation() : operation_argument;
    const aes::ops::EncryptionOperations& encryption_operation = request_file_or_message_operation();
    std::string message = (message_argument.empty()) ? request_message() : message_argument;
    const AES& aes = (aes_argument == aes::def::DEFAULT_AES) ? request_aes_type() : aes_argument;
    const aes::mod::Modes& mode = (mode_argument == aes::def::DEFAULT_MODE) ? request_mode() : mode_argument;
    const aes::pad::Paddings& padding = (padding_argument == aes::def::DEFAULT_PADDING) ? request_padding() : padding_argument;
    std::string key = (key_argument.empty()) ? request_key() : key_argument;
    const std::string& input_file_path = (input_file_path_argument.empty()) ? request_input_file() : input_file_path_argument;
    const std::string& output_file_path = (output_file_path_argument == aes::def::DEFAULT_OUTPUT_FILE_PATH) ? request_output_file() : output_file_path_argument;
    std::vector<uint8_t> iv;

    if(mode_argument != aes::mod::Modes::ECB) {
        iv = (iv_argument.empty()) ? request_iv(mode) : std::vector<uint8_t>(iv_argument.cbegin(), iv_argument.cend());
    }

    execute_message_or_file(encryption_operation, operation, message, key, aes, mode, padding, iv, input_file_path, output_file_path);

    //TODO: volendo potrei far ritornare Operations a get_user_input() e fare il resto qui.
    get_user_input();
}

//TODO: in get_user_input() posso volendo solo mettere le chiamate alle altre funzioni.
void get_user_input()
{
    //TODO: a questo punto, quasi rimuovere la funzione get_user_input() e semplicemente spostare questo codice in show_console();
    operation_encryption_decryption(request_operation());
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

void operation_encryption_decryption(const ops::Operations& operation)
{
    unsigned short encryption_decryption_operation;
    //TODO: prima era: Su cosa si desidera eseguire questa operazione?
    std::cout << (operation == ops::Operations::ENCRYPT ? "Cosa si desidera cifrare?" : "Cosa si desidera decifrare?") << '\n';
    for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
        std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> encryption_decryption_operation;
    while((std::cin.fail()) || (encryption_decryption_operation < aes::ops::get_operation_index(aes::ops::Operations::ENCRYPT) || encryption_decryption_operation > aes::ops::get_operation_index(aes::ops::Operations::DECRYPT))) {
        std::cin.clear();
        std::cout << (operation == ops::Operations::ENCRYPT ? "Cosa si desidera cifrare?" : "Cosa si desidera decifrare?") << '\n';
        for(const auto& e : aes::ops::ALL_ENCRYPTION_OPERATIONS) {
            std::cout << static_cast<unsigned short>(aes::ops::get_operation_index(e)) << ". " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> encryption_decryption_operation;
        std::cout << std::flush;
        AES_DEBUG("encryption_decryption_operation: {}", encryption_decryption_operation)
    }

    std::cin.clear();

    const aes::ops::EncryptionOperations& encryption_decryption_operation_selected = static_cast<aes::ops::EncryptionOperations>(encryption_decryption_operation);
    std::cout << "Operazione selezionata: " << aes::ops::ENCRYPTION_OPERATIONS_NAMES.at(encryption_decryption_operation_selected) << std::endl;

    switch(encryption_decryption_operation_selected) {
        case aes::ops::EncryptionOperations::MESSAGE:
            AES_INFO("Inside MESSAGE case, encryption_decryption_operation: {}", encryption_decryption_operation)
            show_encrypt_decrypt_message(operation);
            break;
        case aes::ops::EncryptionOperations::FILE:
            AES_INFO("Inside FILE case, encryption_decryption_operation: {}", encryption_decryption_operation)
            show_encrypt_decrypt_file(operation);
            break;
        default:
            AES_CRITICAL("encryption_decryption_operation should not be in default case, encryption_decryption_operation: {}", encryption_decryption_operation)
            exit(EXIT_FAILURE);
    }
}

//TODO: optional iv?
void execute_message_or_file(const aes::ops::EncryptionOperations& encryption_operation, const aes::ops::Operations& operation, std::string& message, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding,
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

void show_encrypt_decrypt_message(const ops::Operations& operation)
{
    std::string message = request_message();
    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    std::string key = request_key(); //TODO: usare magari una classe Key al posto di una stringa.
    const std::vector<uint8_t>& iv = request_iv(mode);
    const aes::AES& aes = request_aes_type();

    AES_DEBUG("aes type: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

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

void show_encrypt_decrypt_file(const ops::Operations& operation) //TODO: remove?
{
    const std::string& input_file_path = request_input_file();
    //const std::vector<char*>& file_data = aes::fm::FileManager::get_file_data(input_file_path); //TODO: remove
    //const std::vector<std::string>& file_data2 = aes::fm::FileManager::get_file_data2(input_file_path); //TODO: remove
    std::string file_data3 = aes::fm::FileManager::get_file_data3(input_file_path); //TODO: rename in file_data

    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    std::string key = request_key();
    const std::vector<uint8_t>& iv = request_iv(mode);
    const aes::AES& aes = request_aes_type();

    const std::string& output_file_path = request_output_file();

    switch(operation) {
        case ops::Operations::ENCRYPT:
        {
            //TODO: volendo usare aes::api::encrypt_file();
            const std::vector<uint8_t>& ciphertext = aes::api::encrypt(file_data3, key, iv, padding, mode, aes);
            AES_DEBUG("ciphertext: {}", std::string(ciphertext.cbegin(), ciphertext.cend()))
            aes::fm::FileManager::write_file_data(output_file_path, ciphertext);
            break;
        }
        case ops::Operations::DECRYPT:
        {
            //TODO: volendo usare aes::api::decrypt_file();
            const std::vector<uint8_t>& deciphered_plaintext = aes::api::decrypt(file_data3, key, iv, padding, mode, aes);
            AES_DEBUG("ciphertext: {}", std::string(deciphered_plaintext.cbegin(), deciphered_plaintext.cend()))
            aes::fm::FileManager::write_file_data(output_file_path, deciphered_plaintext);
            break;
        }
    }
}

void encrypt_decrypt_file(const aes::ops::Operations& operation, std::string input_file_data, const std::string& output_file_path, std::string& key, const aes::AES& aes, const aes::mod::Modes& mode, const aes::pad::Paddings& padding, const std::vector<uint8_t>& iv)
{
    switch(operation) {
        case ops::Operations::ENCRYPT:
        {
            //TODO: volendo usare aes::api::encrypt_file();
            /*const std::vector<uint8_t>& ciphertext = aes::api::encrypt(input_file_data, key, iv, padding, mode, aes); //TODO: remove
            AES_DEBUG("ciphertext: {}", std::string(ciphertext.cbegin(), ciphertext.cend()))
            aes::fm::FileManager::write_file_data(output_file_path, ciphertext);*/
            aes::api::encrypt_file(input_file_data, output_file_path, key, iv, padding, mode, aes);
            break;
        }
        case ops::Operations::DECRYPT:
        {
            //TODO: volendo usare aes::api::decrypt_file();
            /*const std::vector<uint8_t>& deciphered_plaintext = aes::api::decrypt(input_file_data, key, iv, padding, mode, aes); //TODO: remove
            AES_DEBUG("ciphertext: {}", std::string(deciphered_plaintext.cbegin(), deciphered_plaintext.cend()))
            aes::fm::FileManager::write_file_data(output_file_path, deciphered_plaintext);*/
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
    //TODO: < ops::get_operation_index(ops::all_encryption_operations.front()) || user_input_operation > ops::all_encryption_operations.size())
    while((std::cin.fail()) || user_input_operation < 1 || user_input_operation > 2) { //!(std::cin >> encryption_operation) || (std::cin.fail())
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

    //TODO: salare la chiave, metterci il pepe, iv, nonce, ecc.
    //TODO: la key può essere o una stringa o una specifica classe. tipo: class Password/Key/KeyGenerator, ecc.
    //TODO: in questa classe che prende come input una stringa, poi ci mette il sale e il pepe e niente, si può fare
    //TODO: get_key(); get_original_key(); get_salt(); get_iv(); get_nonce(); ecc.

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

    //TODO: prima era get_mode_index(aes::mod::Modes::CFB))
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
