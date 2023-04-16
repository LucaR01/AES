//
// Created by Luca on 01/03/2023.
//

#include <iostream>
#include <map>
#include <utility>

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
        //std::cout << arg.first << " : " << arg.second << std::endl; //TODO: uncomment
        AES_DEBUG("{} : {}", arg.first, arg.second.isString() ? arg.second.asString() : arg.second.isBool() ? std::to_string(arg.second.asBool()) : "empty") //TODO: to fix.
    }
#endif

    std::pair<aes::AES, Arguments> aes;
    args["--aes"] && args["--aes"].isString() ? aes = std::make_pair(aes::cvt::retrieve_key_from_map<aes::AES, std::string_view>(aes::ALL_AES_TYPES_NAMES, args["--aes"].asString()).value(), Arguments::USER_PASSED_ARGUMENT) : aes = std::make_pair(aes::def::DEFAULT_AES, Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("aes: {} : {}", aes::ALL_AES_TYPES_NAMES.at(aes.first), ARGUMENTS_NAMES.at(aes.second))

    std::pair<std::string, Arguments> message;
    args["--message"] && args["--message"].isString() ? message = std::make_pair(args["--message"].asString(), Arguments::USER_PASSED_ARGUMENT) : message = std::make_pair("", Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("message: {} : {}", message.first, ARGUMENTS_NAMES.at(message.second))

    std::pair<std::string, Arguments> input_file_path;
    args["--input"] && args["--input"].isString() ? input_file_path = std::make_pair(args["--input"].asString(), Arguments::USER_PASSED_ARGUMENT) : input_file_path = std::make_pair("", Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("input_path_string: {} : {}", input_file_path.first, ARGUMENTS_NAMES.at(input_file_path.second))

    std::pair<std::string, Arguments> output_file_path;
    args["--output"] && args["--output"].isString() ? output_file_path = std::make_pair(args["--output"].asString(), Arguments::USER_PASSED_ARGUMENT) : output_file_path = std::make_pair(aes::def::DEFAULT_OUTPUT_FILE_PATH, Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("output_path_string: {} : {}", output_file_path.first, ARGUMENTS_NAMES.at(output_file_path.second))

    std::pair<aes::mod::Modes, Arguments> mode;
    args["--mode"] && args["--mode"].isString() ? mode = std::make_pair(aes::cvt::retrieve_key_from_map<aes::mod::Modes, std::string_view>(aes::mod::MODES_NAMES, std::string_view(args["--mode"].asString())).value(), Arguments::USER_PASSED_ARGUMENT) : mode = std::make_pair(aes::def::DEFAULT_MODE, Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("mode: {} : {}", aes::mod::MODES_NAMES.at(mode.first), ARGUMENTS_NAMES.at(mode.second))

    std::pair<aes::pad::Paddings, Arguments> padding;
    args["--padding"] && args["--padding"].isString() ? padding = std::make_pair(aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(args["--padding"].asString())).value(), Arguments::USER_PASSED_ARGUMENT) : padding = std::make_pair(aes::def::DEFAULT_PADDING, Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("padding: {} : {}", aes::pad::PADDING_NAMES.at(padding.first), ARGUMENTS_NAMES.at(padding.second))

    std::pair<std::string, Arguments> iv;
    args["--iv"] && args["--iv"].isString() ? iv = std::make_pair(args["--iv"].asString(), Arguments::USER_PASSED_ARGUMENT) : std::make_pair("", Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("iv: {} : {}", iv.first, ARGUMENTS_NAMES.at(iv.second))

    std::pair<std::string, Arguments> key;
    args["--key"] && args["--key"].isString() ? key = std::make_pair(args["--key"].asString(), Arguments::USER_PASSED_ARGUMENT) : std::make_pair("", Arguments::NOT_USER_PASSED_ARGUMENT);
    AES_DEBUG("key: {} : {}", key.first, ARGUMENTS_NAMES.at(key.second))

    std::pair<aes::ops::Operations, Arguments> operation;

    if(args["--encryption"].asBool()) { // prima args["--encryption"] &&
        operation = std::make_pair(aes::ops::Operations::ENCRYPT, Arguments::USER_PASSED_ARGUMENT);
    } else if(args["--decryption"].asBool()) {
        operation = std::make_pair(aes::ops::Operations::DECRYPT, Arguments::USER_PASSED_ARGUMENT);
    } else if(args["--operation"] && args["--operation"].isString()) {
        operation = std::make_pair(aes::cvt::retrieve_key_from_map(aes::ops::OPERATIONS_NAMES, std::string_view(args["--operation"].asString())).value_or(aes::def::DEFAULT_OPERATION), Arguments::NOT_USER_PASSED_ARGUMENT);
    } else {
        operation = std::make_pair(aes::def::DEFAULT_OPERATION, Arguments::NOT_USER_PASSED_ARGUMENT);
    }

    AES_DEBUG("operation: {} : {}", aes::ops::OPERATIONS_NAMES.at(operation.first), ARGUMENTS_NAMES.at(operation.second))


    if((args["-g"] && args["-g"].isBool() && args["-g"].asBool()) || (args["--gui"] && args["--gui"].isBool() && args["--gui"].asBool())) {
        aes::gui::show(aes.first, mode.first, padding.first, message.first, key.first, iv.first, input_file_path.first, output_file_path.first);
    } else {
        // Se non viene dato l'opzione -c o --console o -g o --gui allora mostrare la schermata di default.
        // O la console o la gui.
        aes::con::show_console(aes, mode, padding, message, key, iv, input_file_path, output_file_path, operation);
    }
}

} // namespace aes::arg