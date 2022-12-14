//
// Created by Luca on 09/12/2022.
//

#include <iostream>
#include <string>

#include "graphics/console/console.hpp"
#include "core/operations/operations.hpp"
#include "logger/logger.hpp"

namespace aes::con {

void show_console()
{
    //TODO: volendo potrei far ritornare Operations a get_user_input() e fare il resto qui.
    get_user_input();
}

void get_user_input()
{
    unsigned short operation_selected; //TODO: renderlo un Operations di default?

    //TODO: volendo aggiungere la possibilità di avere più lingue.
    std::cout << "Seleziona l'operazione:" << '\n';
    std::cout << "1. Cifratura " << '\n';
    std::cout << "2. Decifrare " << std::endl;
    std::cout << "[Input: 1|2] Scelta: ";

    std::cin >> operation_selected;
    AES_TRACE("operation_selected: {}", operation_selected)
    //TODO: cambiare la condizione nel while.
    while(operation_selected != get_operation_index(aes::ops::Operations::ENCRYPT) && operation_selected != get_operation_index(aes::ops::Operations::DECRYPT)) {
        std::cout << "[Input: 1|2] Scelta: ";
        std::cin >> operation_selected;
        std::cout << std::flush;
        AES_TRACE("mode_selected: {}", operation_selected)
    }

    std::cin.clear(); //TODO: remove?

    operation_selected = static_cast<aes::ops::Operations>(operation_selected);
    AES_DEBUG("operation_selected converted to enum: {}", operation_selected)

    std::cout << "Operazione selezionata: " << aes::ops::enums_names.at(static_cast<aes::ops::Operations>(operation_selected)) << std::endl;

    switch(operation_selected) {
        case aes::ops::ENCRYPT:
            AES_INFO("Inside ENCRYPT case, operation_selected: {}", operation_selected)
            operation_encryption();
            break;
        case aes::ops::DECRYPT:
            AES_INFO("Inside DECRYPT case, operation_selected: {}", operation_selected)
            operation_decryption();
            break;
        default:
            AES_ERROR("operation_selected should not be in default branch, operation_selected: {}", operation_selected)
            exit(0);
    }
}

void operation_encryption()
{
    unsigned short encryption_operation;
    //TODO: chiedere messaggio, chiave, modalità, padding.
    //TODO: chiedere file input, path output file, estensione del file?, chiave, modalità, padding.
    std::cout << "Cosa si desidera cifrare?" << '\n';
    std::cout << "1. Messaggio" << '\n';
    std::cout << "2. File" << '\n';
    std::cout << "Seleziona: ";
    std::cin >> encryption_operation;
    while((std::cin.fail()) || (encryption_operation <= 0 || encryption_operation >= 3)) { //!(std::cin >> encryption_operation) || (std::cin.fail())
        std::cin.clear();
        std::cout << "Cosa si desidera cifrare?" << '\n';
        std::cout << "1. Messaggio" << '\n';
        std::cout << "2. File" << '\n';
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
            exit(0);
    }
}

void show_encrypt_message()
{
    //std::string_view message; //TODO: remove
    std::string message;
    std::cout << "Inserire messaggio: ";
    std::cin.ignore();
    std::getline(std::cin, message);
    AES_DEBUG("Message from user input: {}", message)

    aes::mod::Modes mode = request_mode();
    aes::pad::Paddings padding = request_padding();
}

void show_encrypt_file()
{

}

void operation_decryption()
{
    //TODO: scelta tra decifratura di un messaggio o di un file.
    //TODO: chiedere messaggio cifrato, chiave, modalità, padding.
    //TODO: chiedere file input cifrato, path output file decifrato, estensione del file?, chiave, modalità, padding.
}

void show_decrypt_message()
{

}

void show_decrypt_file()
{

}

aes::pad::Paddings request_padding()
{
    unsigned short padding_type;
    std::cout << "Seleziona tipo di padding: " << '\n';
    std::cout << "1. No padding" << '\n';
    std::cout << "2. PKCS5" << '\n';
    std::cout << "Seleziona: ";
    std::cin >> padding_type;

    while((std::cin.fail()) || (padding_type < 1 || padding_type > 2)) {
        std::cin.clear();
        std::cout << "Seleziona tipo di padding: " << '\n';
        std::cout << "1. No padding" << '\n';
        std::cout << "2. PKCS5" << '\n';
        std::cout << "Seleziona: " << std::flush;
        std::cin >> padding_type;
        AES_DEBUG("padding_type: {}", padding_type)
    }

    aes::pad::Paddings selected_padding_type = static_cast<aes::pad::Paddings>(padding_type);
    std::cout << "Hai selezionato: " << aes::pad::padding_names.at(selected_padding_type) << std::endl;
    return selected_padding_type;
}

std::string_view request_key()
{
    //TODO:
    return "";
}

aes::mod::Modes request_mode()
{
    unsigned short mode;
    std::cout << "Seleziona modalità: " << '\n';
    //TODO: potrei usare l'enum per stampare i vari tipi, piuttosto che passarli io così direttamente.
    std::cout << "0. ECB" << '\n';
    std::cout << "1. CBC" << '\n';
    std::cout << "2. CTR" << '\n';
    std::cout << "Seleziona: ";
    std::cin >> mode;

    while((std::cin.fail()) || (mode < aes::mod::get_mode_index(aes::mod::Modes::ECB) || mode > aes::mod::get_mode_index(aes::mod::Modes::CTR))) {
        std::cin.clear();
        std::cout << "0. ECB" << '\n';
        std::cout << "1. CBC" << '\n';
        std::cout << "2. CTR" << '\n';
        std::cout << "Seleziona: ";
        std::cin >> mode;
        AES_DEBUG("mode: {}", mode)
    }

    aes::mod::Modes selected_mode = static_cast<aes::mod::Modes>(mode);
    std::cout << "Hai selezionato: " << aes::mod::modes_names.at(selected_mode) << std::endl;
    return selected_mode;
}

} //namespace aes::con
