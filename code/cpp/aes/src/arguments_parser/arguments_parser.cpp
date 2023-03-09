//
// Created by Luca on 01/03/2023.
//

#include <iostream>
#include <map>
#include <tuple>

#include "arguments_parser/arguments_parser.hpp"
#include "version/version.hpp"
#include "graphics/gui/gui.hpp"
#include "graphics/console/console.hpp"
#include "convert/aes_convert.hpp"
#include "logger/logger.hpp"
#include "defaults/defaults.hpp"

namespace aes::arg {

void parse_user_arguments(const int& argc, const char** argv)
{
    std::map< std::string, docopt::value > args = docopt::docopt(aes::arg::USAGE,{argv + 1, argv + argc}, true, aes::vrs::aes.version);

#ifndef RELEASE_MODE
    for(const auto& arg : args) {
        std::cout << arg.first << " : " << arg.second << std::endl;
        //AES_DEBUG("{} : ", arg.first) //TODO: to fix.
    }
#endif

    //TODO: iv;
    //TODO: controllare se gli argomenti sono corretti nelle variabili.

    aes::AES aes;
    args["aes"] && args["aes"].isString() ? aes = aes::cvt::retrieve_key_from_map<aes::AES, std::string_view>(aes::ALL_AES_TYPES_NAMES, args["aes"].asString()).value() : aes = aes::def::DEFAULT_AES;
    AES_DEBUG("aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

    std::string message;
    //TODO: provare a togliere il check .isString();
    //TODO: <message> e basta funziona; nello usage c'è da mettere [<message>] e basta senza il -m, però.
    args["<message>"] && args["<message>"].isString() ? message = args["<message>"].asString() : message = "";
    AES_DEBUG("message: {}", message)

    std::string input_path_string;
    args["-i"] && args["-i"].isString() ? input_path_string = args["-i"].asString() : input_path_string = "";
    AES_DEBUG("input_path_string: {}", input_path_string)

    std::string output_path_string;
    args["-o"] && args["-o"].isString() ? output_path_string = args["-o"].asString() : output_path_string = aes::def::DEFAULT_OUTPUT_FILE_PATH;
    AES_DEBUG("output_path_string: {}", output_path_string)

    aes::mod::Modes mode;
    args["--mode"] && args["--mode"].isString() ? mode = aes::cvt::retrieve_key_from_map<aes::mod::Modes, std::string_view>(aes::mod::MODES_NAMES, std::string_view(args["--mode"].asString())).value() : mode = aes::def::DEFAULT_MODE;
    AES_DEBUG("mode: {}", aes::mod::MODES_NAMES.at(mode))

    aes::pad::Paddings padding;
    args["-p"] && args["-p"].isString() ? padding = aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(args["-p"].asString())).value() : padding = aes::def::DEFAULT_PADDING;
    AES_DEBUG("padding: {}", aes::pad::PADDING_NAMES.at(padding))

    aes::ops::Operations operation;

    //TODO: refactor this part.
    //TODO: remove
    /*if(args["--encryption"] && !args["--encryption"].asBool() && args["--decryption"] && !args["--decryption"].asBool()) {
        operation = aes::def::DEFAULT_OPERATION;
    } else if(args["--encryption"].asBool()) {
        operation = aes::ops::Operations::ENCRYPT;
    } else if(args["--decryption"].asBool()) {
        operation = aes::ops::Operations::DECRYPT;
    } else if(args["--operation"] && args["--operation"].isString()) {
        operation = aes::cvt::retrieve_key_from_map(aes::ops::OPERATIONS_NAMES, std::string_view(args["--operation"].asString())).value_or(aes::def::DEFAULT_OPERATION);
    } else {
        operation = aes::def::DEFAULT_OPERATION;
    }*/

    //TODO: refactor
    if(args["--encryption"].asBool()) { // prima args["--encryption"] &&
        operation = aes::ops::Operations::ENCRYPT;
    } else if(args["--decryption"].asBool()) {
        operation = aes::ops::Operations::DECRYPT;
    } else if(args["--operation"] && args["--operation"].isString()) {
        operation = aes::cvt::retrieve_key_from_map(aes::ops::OPERATIONS_NAMES, std::string_view(args["--operation"].asString())).value_or(aes::def::DEFAULT_OPERATION);
    } else {
        operation = aes::def::DEFAULT_OPERATION;
    }

    AES_DEBUG("operation: {}", aes::ops::OPERATIONS_NAMES.at(operation))


    if((args["-g"] && args["-g"].isBool() && args["-g"].asBool()) || (args["--gui"] && args["--gui"].isBool() && args["--gui"].asBool())) {
        aes::gui::show();
    } else {
        // Se non viene dato l'opzione -c o --console o -g o --gui allora mostrare la schermata di default.
        // O la console o la gui.
        aes::con::show_console();
    }
}

//TODO: manca l'iv.
/*void get_user_arguments(const docopt::value& aes_type, const docopt::value& input_message, const docopt::value& input_path, const docopt::value& output_path,
                        const docopt::value& mode_string, const docopt::value& padding_string, const docopt::value& operation_type,
                        const docopt::value& is_encryption, const docopt::value& is_decryption)
{
    //TODO: prima di assegnarlo controllare se è presente questo valore inserito dall'utente. (non serve, già fatto).
    //TODO: std::find o aes::ALL_AES_TYPES_NAMES.find(); poi se è presente assegnarlo.
    //TODO: allo show_console passare tutto, mentre al show_gui() non serve, per esempio, passare encrypt o decrypt, perché vengono mostrate entrambe a prescindere.

    aes::AES aes;
    aes_type && aes_type.isString() ? aes = aes::cvt::retrieve_key_from_map<aes::AES, std::string_view>(aes::ALL_AES_TYPES_NAMES, aes_type.asString()).value() : aes = aes::def::DEFAULT_AES;
    AES_DEBUG("aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

    std::string message;
    input_message && input_message.isString() ? message = input_message.asString() : message = "";
    AES_DEBUG("message: {}", message)

    std::string input_path_string;
    input_path && input_path.isString() ? input_path_string = input_path.asString() : input_path_string = "";
    AES_DEBUG("input_path_string: {}", input_path_string)

    std::string output_path_string;
    output_path && output_path.isString() ? output_path_string = output_path.asString() : output_path_string = aes::def::DEFAULT_OUTPUT_FILE_PATH;
    AES_DEBUG("output_path_string: {}", output_path_string)

    aes::mod::Modes mode;
    mode_string && mode_string.isString() ? mode = aes::cvt::retrieve_key_from_map<aes::mod::Modes, std::string_view>(aes::mod::MODES_NAMES, std::string_view(mode_string.asString())).value() : mode = aes::def::DEFAULT_MODE;
    AES_DEBUG("mode: {}", aes::mod::MODES_NAMES.at(mode))

    aes::pad::Paddings padding;
    padding_string && padding_string.isString() ? padding = aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(padding_string.asString())).value() : padding = aes::def::DEFAULT_PADDING;
    AES_DEBUG("padding: {}", aes::pad::PADDING_NAMES.at(padding))

    aes::ops::EncryptionOperations operation;
    //bool is_encryption;
    //bool is_decryption;

    //TODO: get "Encryption" or "Decryption" se è una stringa oppure ottenere encryption: true o false o decryption: true o false

    if(operation_type) {
        if(operation_type.isString()) {
            operation = aes::cvt::retrieve_key_from_map(aes::ops::ENCRYPTION_OPERATIONS_NAMES, std::string_view(operation_type.asString())).value_or(aes::def::DEFAULT_ENCRYPTION_OPERATION);
        } else if(operation_type.isBool()) {
            //is_encryption = operation_type.asBool();
        }
    }

    if(is_encryption && is_encryption.isBool() || is_decryption && is_decryption.isBool()) {
        //is_encryption.asBool() ? user_arguments.add(is_encryption.asBool()) : is_decryption.asBool() ? user_arguments.add(is_decryption.asBool()) : user_arguments.add(operation);

    }

    std::tuple user_arguments(aes, message, input_path_string, output_path_string, mode, padding, operation);
}*/

} // namespace aes::arg