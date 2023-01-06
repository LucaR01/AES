//
// Created by Luca on 09/12/2022.
//

#include <iostream>
#include <string>

#include "graphics/console/console.hpp"
#include "logger/logger.hpp"
#include "file_manager/file_manager.hpp"

namespace aes::con {

//TODO: usare uint8_t dove possibile.

void show_console()
{
    //TODO: volendo potrei far ritornare Operations a get_user_input() e fare il resto qui.
    get_user_input();
}

//TODO: in get_user_input() posso volendo solo mettere le chiamate alle altre funzioni.
void get_user_input()
{
    /*aes::ops::Operations operation = request_operation();
    operation_encryption_decryption(operation);*/

    //TODO: a questo punto, quasi rimuovere la funzione get_user_input() e semplicemente spostare questo codice in show_console();

    operation_encryption_decryption(request_operation());

    /*switch(operation) { //TODO: remove?
        case aes::ops::ENCRYPT:
            AES_INFO("Inside ENCRYPT case, operation_selected: {}", aes::ops::operations_names.at(operation))
            operation_encryption();
            break;
        case aes::ops::DECRYPT:
            AES_INFO("Inside DECRYPT case, operation_selected: {}", aes::ops::operations_names.at(operation))
            operation_decryption();
            break;
        default:
            AES_ERROR("operation_selected should not be in default branch, operation_selected: {}", aes::ops::operations_names.at(operation))
            exit(EXIT_FAILURE);
    }*/
}

//TODO: però questa la utilizzo una singola volta, la tengo lo stesso? Per il principio di Single Responsability Principle.
aes::ops::Operations request_operation()
{
    unsigned short operation_selected; //TODO: renderlo un Operations di default?

    //TODO: volendo aggiungere la possibilità di avere più lingue.
    std::cout << "Seleziona l'operazione:" << '\n';
    for(const auto& op : aes::ops::all_operations) {
        std::cout << aes::ops::get_operation_index(op) << ". " << aes::ops::operations_names.at(op) << '\n';
    }
    std::cout << "[Input: 1|2] Scelta: ";

    std::cin >> operation_selected;
    AES_TRACE("operation_selected: {}", operation_selected)
    //TODO: cambiare la condizione nel while.
    while((std::cin.fail()) || (operation_selected < aes::ops::get_operation_index(aes::ops::all_operations.front()) || operation_selected > aes::ops::all_operations.size())) {
        //std::cout << "[Input: 1|2] Scelta: "; //TODO: uncomment se lascio solo questo al posto di quello sotto.
        for(const auto& op : aes::ops::all_operations) { //TODO: forse è quasi meglio non ristamparli. (non serve, sono già sopra)
            std::cout << aes::ops::get_operation_index(op) << ". " << aes::ops::operations_names.at(op) << '\n';
        }
        std::cout << "[Input: 1|2] Scelta: ";
        std::cin >> operation_selected;
        std::cout << std::flush;
        AES_TRACE("mode_selected: {}", operation_selected)
    }

    std::cin.clear(); //TODO: remove?

    aes::ops::Operations op = static_cast<aes::ops::Operations>(operation_selected);
    std::cout << "Operazione selezionata: " << aes::ops::operations_names.at(op) << std::endl;

    return op;
}

// ENCRYPTION

void operation_encryption()
{
    unsigned short encryption_operation;
    //TODO: chiedere messaggio, chiave, modalità, padding.
    //TODO: chiedere file input, path output file, estensione del file?, chiave, modalità, padding.
    std::cout << "Cosa si desidera cifrare?" << '\n';
    for(const auto& e : aes::ops::all_encryption_operations) {
        std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> encryption_operation;
    while((std::cin.fail()) || (encryption_operation <= 0 || encryption_operation >= 3)) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << "Cosa si desidera cifrare?" << '\n';
        for(const auto& e : aes::ops::all_encryption_operations) {
            std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> encryption_operation;
        std::cout << std::flush;
        AES_DEBUG("encryption_operation: {}", encryption_operation)
    }

    std::cin.clear();

    aes::ops::EncryptionOperations encryption_operation_selected = static_cast<aes::ops::EncryptionOperations>(encryption_operation);
    std::cout << "Operazione selezionata: " << aes::ops::encryption_operations_names.at(encryption_operation_selected) << std::endl;

    switch(encryption_operation_selected) {
        case aes::ops::EncryptionOperations::MESSAGE:
            AES_INFO("Inside MESSAGE case, encryption_operation: {}", encryption_operation)
            show_encrypt_message();
            break;
        case aes::ops::EncryptionOperations::FILE:
            AES_INFO("Inside FILE case, encryption_operation: {}", encryption_operation)
            show_encrypt_file();
            break;
        default:
            AES_CRITICAL("encryption_operation should not be in default case, encryption_operation: {}", encryption_operation)
            exit(EXIT_FAILURE);
    }
}

void show_encrypt_message()
{
    const std::string& message = request_message();
    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    const std::string& key = request_key(); //TODO: usare magari una classe al posto di una stringa.
    //TODO: chiamare funzione per cifrare
    //TODO: mostrare a schermo il messaggio cifrato.
}

void show_encrypt_file()
{
    const std::string& input_file_path = request_input_file();
    const std::vector<char*>& file_data = aes::fm::FileManager::get_file_data(input_file_path); //TODO: o questo
    const std::vector<std::string>& file_data2 = aes::fm::FileManager::get_file_data2(input_file_path); //TODO: o questo
    const std::string& file_data3 = aes::fm::FileManager::get_key(input_file_path); //TODO: oppure questo.

    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    const std::string& key = request_key(); //TODO: questo forse da cambiare.

    const std::string& output_file_path = request_output_file();

    //TODO: chiamare la funzione per cifrare
    //TODO: cifrare i dati e scriverli in un altro file.
}

// DECRYPTION

void operation_decryption()
{
    //TODO: scelta tra decifratura di un messaggio o di un file.
    //TODO: chiedere messaggio cifrato, chiave, modalità, padding.
    //TODO: chiedere file input cifrato, path output file decifrato, estensione del file?, chiave, modalità, padding.

    unsigned short decryption_operation;
    std::cout << "Cosa si desidera decifrare?" << '\n';
    for(const auto& e : aes::ops::all_encryption_operations) {
        std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> decryption_operation;
    while((std::cin.fail()) || (decryption_operation <= 0 || decryption_operation >= 3)) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << "Cosa si desidera decifrare?" << '\n';
        for(const auto& e : aes::ops::all_encryption_operations) {
            std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> decryption_operation;
        std::cout << std::flush;
        AES_DEBUG("decryption_operation: {}", decryption_operation)
    }

    std::cin.clear();

    aes::ops::EncryptionOperations decryption_operation_selected = static_cast<aes::ops::EncryptionOperations>(decryption_operation);
    std::cout << "Operazione selezionata: " << aes::ops::encryption_operations_names.at(decryption_operation_selected) << std::endl;

    switch(decryption_operation_selected) {
        case aes::ops::EncryptionOperations::MESSAGE:
            AES_INFO("Inside MESSAGE case, decryption_operation: {}", decryption_operation)
            show_decrypt_message();
            break;
        case aes::ops::EncryptionOperations::FILE:
            AES_INFO("Inside FILE case, decryption_operation: {}", decryption_operation)
            show_decrypt_file();
            break;
        default:
            AES_CRITICAL("decryption_operation should not be in default case, decryption_operation: {}", decryption_operation)
            exit(EXIT_FAILURE);
    }
}

void show_decrypt_message()
{
    const std::string& message = request_message();
    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    const std::string& key = request_key(); //TODO: questo forse da cambiare.
    //TODO: chiamare funzione per decifrare
    //TODO: mostrare a schermo il messaggio decifrato.
}

void show_decrypt_file()
{

}

// ENCRYPTION & DECRYPTION

void operation_encryption_decryption(const ops::Operations& operation)
{
    unsigned short encryption_decryption_operation;
    //TODO: prima era: Su cosa si desidera eseguire questa operazione?
    std::cout << (operation == ops::Operations::ENCRYPT ? "Cosa si desidera cifrare?" : "Cosa si desidera decifrare?") << '\n';
    for(const auto& e : aes::ops::all_encryption_operations) {
        std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> encryption_decryption_operation;
    //TODO: 0 e 3 sono magic numbers, usare gli enums
    while((std::cin.fail()) || (encryption_decryption_operation <= 0 || encryption_decryption_operation >= 3)) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << (operation == ops::Operations::ENCRYPT ? "Cosa si desidera cifrare?" : "Cosa si desidera decifrare?") << '\n';
        for(const auto& e : aes::ops::all_encryption_operations) {
            std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> encryption_decryption_operation;
        std::cout << std::flush;
        AES_DEBUG("encryption_decryption_operation: {}", encryption_decryption_operation)
    }

    std::cin.clear();

    const aes::ops::EncryptionOperations& encryption_decryption_operation_selected = static_cast<aes::ops::EncryptionOperations>(encryption_decryption_operation);
    std::cout << "Operazione selezionata: " << aes::ops::encryption_operations_names.at(encryption_decryption_operation_selected) << std::endl;

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

void show_encrypt_decrypt_message(const ops::Operations& operation)
{
    const std::string& message = request_message();
    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    const std::string& key = request_key(); //TODO: usare magari una classe al posto di una stringa.

    switch(operation) {
        case ops::Operations::ENCRYPT:
            //TODO: chiamare funzione per cifrare.
            //TODO: mostrare a schermata il risultato della cifratura.
            break;
        case ops::Operations::DECRYPT:
            //TODO: chiamare funzione per decifrare.
            //TODO: mostrare a schermata il risultato della decifratura.
            break;
    }
}

void show_encrypt_decrypt_file(const ops::Operations& operation) //TODO: remove?
{
    const std::string& input_file_path = request_input_file();
    const std::vector<char*>& file_data = aes::fm::FileManager::get_file_data(input_file_path); //TODO: o questo
    const std::vector<std::string>& file_data2 = aes::fm::FileManager::get_file_data2(input_file_path); //TODO: o questo
    const std::string& file_data3 = aes::fm::FileManager::get_key(input_file_path); //TODO: oppure questo.

    const aes::mod::Modes& mode = request_mode();
    const aes::pad::Paddings& padding = request_padding();
    const std::string& key = request_key(); //TODO: questo forse da cambiare.

    const std::string& output_file_path = request_output_file();

    switch(operation) {
        case ops::Operations::ENCRYPT:
            //TODO: chiamare funzione per cifrare il file.
            break;
        case ops::Operations::DECRYPT:
            //TODO: chiamare funzione per decifrare il file.
            break;
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
    for(const auto& p : aes::pad::all) {
        std::cout << aes::pad::get_padding_index(p) << ". " << aes::pad::padding_names.at(p) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> padding_type;

    while((std::cin.fail()) || (padding_type < 1 || padding_type > 2)) {
        std::cin.clear();
        std::cout << "Seleziona tipo di padding: " << '\n';
        for(const auto& p : aes::pad::all) {
            std::cout << aes::pad::get_padding_index(p) << ". " << aes::pad::padding_names.at(p) << '\n';
        }
        std::cout << "Seleziona: " << std::flush;
        std::cin >> padding_type;
        AES_DEBUG("padding_type: {}", padding_type)
    }

    aes::pad::Paddings selected_padding_type = static_cast<aes::pad::Paddings>(padding_type);
    std::cout << "Hai selezionato: " << aes::pad::padding_names.at(selected_padding_type) << std::endl;
    return selected_padding_type;
}

std::string request_key()
{
    unsigned short user_input_operation;
    std::cout << "Inserire chiave da input o da file?" << '\n'; //TODO: riscrivere?
    for(const auto& e : aes::ops::all_encryption_operations) {
        std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
    }

    std::cout << "Seleziona: ";
    std::cin >> user_input_operation;
    //TODO: < ops::get_operation_index(ops::all_encryption_operations.front()) || user_input_operation > ops::all_encryption_operations.size())
    while((std::cin.fail()) || user_input_operation < 1 || user_input_operation > 2) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << "Inserire chiave da input o da file?" << '\n';
        for(const auto& e : aes::ops::all_encryption_operations) {
            std::cout << aes::ops::get_operation_index(e) << ". " << aes::ops::encryption_operations_names.at(e) << '\n';
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
    for(const auto& m : aes::mod::all) {
        std::cout << aes::mod::get_mode_index(m) << ". " << aes::mod::modes_names.at(m) << '\n';
    }
    std::cout << "Seleziona: ";
    std::cin >> mode;

    while((std::cin.fail()) || (mode < aes::mod::get_mode_index(aes::mod::Modes::ECB) || mode > aes::mod::get_mode_index(aes::mod::Modes::CTR))) {
        std::cin.clear();
        for(const auto& e : aes::mod::all) {
            std::cout << aes::mod::get_mode_index(e) << ". " << aes::mod::modes_names.at(e) << '\n';
        }
        std::cout << "Seleziona: ";
        std::cin >> mode;
        AES_DEBUG("mode: {}", mode)
    }

    aes::mod::Modes selected_mode = static_cast<aes::mod::Modes>(mode);
    std::cout << "Hai selezionato: " << aes::mod::modes_names.at(selected_mode) << std::endl;
    return selected_mode;
}

} //namespace aes::con
