//
// Created by Luca on 01/03/2023.
//

#ifndef AES_ARGUMENTS_PARSER_HPP
#define AES_ARGUMENTS_PARSER_HPP

#include <variant>

#include "docopt.h"

namespace aes::arg {

//TODO: fix message or file or keep it as it is.
//TODO: aggiungere la possibilità di avere una key.
//TODO: -m MESSAGE, --message=MESSAGE (mi serve un tot di parole non solo una)
//TODO: -i FILE, --input=FILE

// Quindi come passare gli argomenti:
// se è una singola lettera, tipo -i usare "" (o forse anche '' [non va!]); se il parametro è una stringa con degli spazi bianchi usare '' (non va) o "" o \ (neanche questo)
// tipo -p "1-0-Padding" o anche -p="1-0-Padding".
// se si usa quelli con -- c'è bisogna per forza dell'= tipo --aes="AES 256" o anche --mode=CBC (anche senza ""), non funziona con --aes "AES 256".
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
  --aes <AES>                               AES Types [default: AES 128] AES 128, AES 192, AES 256.
  --mode <mode>                             Input Mode [default: ECB] ECB, CBC, CFB.
  -p <padding> --padding <padding>          Input Padding. [default: NO_PADDING] NO_PADDING, 1-0-Padding, ...
  --operation <operation>                   Input Operation. [default: Encrypt] Encrypt, Decrypt.
  --encryption                              Encryption.
  --decryption                              Decryption.
  --iv=IV                                   Input iv.
  -k <key> --key <key>                      Input key.

)";

//TODO: rename?
void parse_user_arguments(const int& argc, const char** argv);

//TODO: remove
/*void get_user_arguments(const docopt::value& aes_type, const docopt::value& input_message, const docopt::value& input_path, const docopt::value& output_path,
                            const docopt::value& mode_string, const docopt::value& padding_string, const docopt::value& operation_type,
                            const docopt::value& is_encryption, const docopt::value& is_decryption);*/

} // namespace aes::arg

#endif //AES_ARGUMENTS_PARSER_HPP
