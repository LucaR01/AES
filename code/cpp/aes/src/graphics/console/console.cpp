//
// Created by Luca on 09/12/2022.
//

#include <iostream>
#include <string>

#include "graphics/console/console.hpp"
#include "logger/logger.hpp"

namespace aes::con {

void show_console()
{
    //TODO: volendo potrei far ritornare Operations a get_user_input() e fare il resto qui.
    get_user_input();
}

//TODO: in get_user_input() posso volendo solo mettere le chiamate alle altre funzioni.
void get_user_input()
{
    aes::ops::Operations operation = request_operation();

    switch(operation) {
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
            exit(0);
    }
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

std::string_view request_key()
{
    //TODO:
    return "";
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
