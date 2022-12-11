//
// Created by Luca on 09/12/2022.
//

#include <iostream>

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
    while(operation_selected != get_operation_index2(Operations::ENCRYPT) && operation_selected != get_operation_index2(Operations::DECRYPT)) {
        std::cout << "[Input: 1|2] Scelta: ";
        std::cin >> operation_selected;
        std::cout << std::flush;
        AES_TRACE("mode_selected: {}", operation_selected)
    }

    operation_selected = static_cast<Operations>(operation_selected);
    AES_DEBUG("operation_selected converted to enum: {}", operation_selected)

    std::cout << "Operazione selezionata: " << enums_names.at(static_cast<Operations>(operation_selected)) << std::endl;

    switch(operation_selected) {
        case ENCRYPT:
            AES_INFO("Inside ENCRYPT case, operation_selected: {}", operation_selected)
            operation_encryption();
            break;
        case DECRYPT:
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
    //TODO: scelta tra cifratura di un messaggio o di un file.
    //TODO: chiedere messaggio, chiave, modalità, padding.
    //TODO: chiedere file input, path output file, estensione del file?, chiave, modalità, padding.
}

void operation_decryption()
{
    //TODO: scelta tra decifratura di un messaggio o di un file.
    //TODO: chiedere messaggio cifrato, chiave, modalità, padding.
    //TODO: chiedere file input cifrato, path output file decifrato, estensione del file?, chiave, modalità, padding.
}

}
