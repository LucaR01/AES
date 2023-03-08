//
// Created by Luca on 01/03/2023.
//

#include <iostream>
#include <map>
#include <variant>

#include "arguments_parser/arguments_parser.hpp"

//#include "logger/logger.hpp" //TODO: remove
#include "version/version.hpp"
#include "graphics/gui/gui.hpp"
#include "graphics/console/console.hpp"
#include "convert/aes_convert.hpp"
#include "logger/logger.hpp"

namespace aes::arg {

void parse_user_arguments(const int& argc, const char** argv)
{
    std::map< std::string, docopt::value > args = docopt::docopt(aes::arg::USAGE,{argv + 1, argv + argc}, true, aes::vrs::aes.version);

#ifndef RELEASE_MODE
    for(const auto& arg : args) {
        std::cout << arg.first << " : " << arg.second << std::endl;
        //AES_DEBUG("{} : {}", arg.first, arg.second) //TODO: doesn't work. remove.
    }
#endif

    if((args["-g"] && args["-g"].isBool() && args["-g"].asBool()) || (args["--gui"] && args["--gui"].isBool() && args["--gui"].asBool())) {
        aes::gui::show();
    } else {
        // Se non viene dato l'opzione -c o --console o -g o --gui allora mostrare la schermata di default.
        // O la console o la gui.
        aes::con::show_console();
    }
}

//TODO: manca l'iv.
void get_user_arguments(const docopt::value& aes_type, const docopt::value& input_message, const docopt::value& input_path, const docopt::value& output_path,
                        const docopt::value& mode_string, const docopt::value& padding_string, const std::variant<docopt::value, docopt::value>& operation_type)
{
    //TODO: prima di assegnarlo controllare se è presente questo valore inserito dall'utente.
    //TODO: std::find o aes::ALL_AES_TYPES_NAMES.find(); poi se è presente assegnarlo.
    //TODO: allo show_console passare tutto, mentre al show_gui() non serve, per esempio, passare encrypt o decrypt, perché vengono mostrate entrambe a prescindere.

    aes::AES aes;
    /*if(aes_type && aes_type.isString()) { //TODO: remove
        for(const auto& [k, v] : aes::ALL_AES_TYPES_NAMES) { //TODO: usare aes::cvt
            if(v == aes_type.asString()) {
                aes = k;
            }
        }
    }*/
    aes_type && aes_type.isString() ? aes = aes::cvt::retrieve_key_from_map<aes::AES, std::string_view>(aes::ALL_AES_TYPES_NAMES, aes_type.asString()).value() : aes = aes::AES::AES_128; //TODO: uncomment and fix.
    AES_DEBUG("aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

    std::string message;
    input_message && input_message.isString() ? message = input_message.asString() : message = "";

    std::string input_path_string;
    input_path && input_path.isString() ? input_path_string = input_path.asString() : input_path_string = "";

    std::string output_path_string;
    output_path && output_path.isString() ? output_path_string = output_path.asString() : output_path_string = "";

    aes::mod::Modes mode;
    mode_string && mode_string.isString() ? mode = aes::cvt::retrieve_key_from_map<aes::mod::Modes, std::string_view>(aes::mod::MODES_NAMES, std::string_view(mode_string.asString())).value() : mode = aes::mod::Modes::ECB; //TODO: uncomment and fix.
    AES_DEBUG("mode: {}", aes::mod::MODES_NAMES.at(mode))

    aes::pad::Paddings padding;
    padding_string && padding_string.isString() ? padding = aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(padding_string.asString())).value() : padding = aes::pad::Paddings::NO_PADDING; //TODO: uncomment and fix.
    AES_DEBUG("padding: {}", aes::pad::PADDING_NAMES.at(padding))

    std::string operation;
    bool is_encryption;
    bool is_decryption;

    //TODO: get "Encryption" or "Decryption" se è una stringa oppure ottenere encryption: true o false o decryption: true o false

    /*if(std::get(operation_type)) { //TODO: fix.
        if(operation_type && operation_type.isString()) {

        } else if(operation_type && operation_type.isBool()) {

        }
    }*/
}

} // namespace aes::arg