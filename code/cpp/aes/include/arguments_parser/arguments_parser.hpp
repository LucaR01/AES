//
// Created by Luca on 01/03/2023.
//

#ifndef AES_ARGUMENTS_PARSER_HPP
#define AES_ARGUMENTS_PARSER_HPP

#include <variant>

#include "docopt.h"

namespace aes::arg {

//TODO: Either monad (std::variant) NOT_USER_PASSED_ARGUMENT or USER_PASSED_ARGUMENT
enum class Arguments {
    NOT_USER_PASSED_ARGUMENT = 0, //TODO: rename in DEFAULT_ARGUMENT?
    USER_PASSED_ARGUMENT
};

static const std::map<Arguments, std::string_view>& ARGUMENTS_NAMES = {
        {Arguments::NOT_USER_PASSED_ARGUMENT, "NOT_USER_PASSED_ARGUMENT"},
        {Arguments::USER_PASSED_ARGUMENT, "USER_PASSED_ARGUMENT"}
};

//TODO: fix message or file or keep it as it is.
//TODO: aggiungere la possibilità di avere una key.
//TODO: -m MESSAGE, --message=MESSAGE (mi serve un tot di parole non solo una)
//TODO: -i FILE, --input=FILE

// Rimuovo i valori di [default], perché altrimenti per come è fatto il programma darebbe sempre USER_PASSED_ARGUMENT

// Quindi come passare gli argomenti:
// se è una singola lettera, tipo -i usare "" (o forse anche '' [non va!]); se il parametro è una stringa con degli spazi bianchi usare '' (non va) o "" o \ (neanche questo)
// tipo -p "1-0-Padding" o anche -p="1-0-Padding".
// se si usa quelli con -- c'è bisogna per forza dell'= tipo --aes="AES 256" o anche --mode=CBC (anche senza ""), non funziona con --aes "AES 256".

// prima era [default: AES 128], [default: ECB], [default: NO_PADDING], [default: Encrypt]
static const char USAGE[] =
        R"(AES.

Usage:
    aes
    aes [-c | --console | -g | --gui]
    aes [--console | --gui] [--aes <AES>] [-p <padding>] [--mode <mode>] [--message <message>] [-i <file>] [-o FILE] [--operation <operation> | --encryption | --decryption] [ --iv=IV ] [--key <key>]

Arguments:
    FILE        optional input file.
    <mode>      mode to be used. [ECB | CBC | CFB]
    <padding>   the padding to be used. [NO_PADDING | 0-Padding | 1-0-Padding | ...]
    <operation> operation. [encryption | decryption]
    IV          optional input iv vector.
    <key>       Optional input key used for encryption or decryption.

Examples:
    --gui --aes="AES 256" --mode=CFB --message="secret message" -p "1-0-Padding" --encryption -i "input.txt" -o "output2.txt" --key="secret key"

Options:
  -h --help                                 Show this screen.
  --version                                 Show version.
  -g --gui                                  Display gui.
  -c --console                              Display console.
  -o FILE --output=FILE                     Output File [default: output.txt].
  -i <file>, --input <file>                 Input File.
  -m <message>, --message <message>         Input message (plaintext or ciphertext).
  --aes <AES>                               AES Types AES 128, AES 192, AES 256.
  --mode <mode>                             Input Mode ECB, CBC, CFB.
  -p <padding> --padding <padding>          Input Padding. NO_PADDING, 1-0-Padding, ...
  --operation <operation>                   Input Operation. Encrypt, Decrypt.
  --encryption                              Encryption.
  --decryption                              Decryption.
  --iv=IV                                   Input iv.
  -k <key> --key <key>                      Input key.

)";

//TODO: rename?
/**
 * @brief: This function reads the user's arguments and either calls @fn show_console() or @fn show_gui() with these parameters.
 * @param argc : the number of arguments passed.
 * @param argv : the arguments.
 */
void parse_user_arguments(const int& argc, const char** argv);

} // namespace aes::arg

#endif //AES_ARGUMENTS_PARSER_HPP
