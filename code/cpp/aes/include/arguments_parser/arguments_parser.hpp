//
// Created by Luca on 01/03/2023.
//

#ifndef AES_ARGUMENTS_PARSER_HPP
#define AES_ARGUMENTS_PARSER_HPP

namespace aes::arg {

static const char USAGE[] =
        R"(AES.

Usage:
    aes
    aes [-c | --console | -g | --gui]
    aes [--console | --gui] [--aes <AES>] [-p <padding>] [--mode <mode] [-m <message>] [-i <file>] [-o FILE] [--operation <operation> | --encryption | --decryption]

Arguments:
    FILE        optional input file.
    <mode>      mode to be used. [ECB | CBC | CFB]
    <padding>   the padding to be used. [NO_PADDING | 0-Padding | 1-0-Padding | ...]
    <operation> operation. [encryption | decryption]

Options:
  -h --help                                 Show this screen.
  --version                                 Show version.
  -g --gui                                  Display gui.
  -c --console                              Display console.
  -o FILE --output=FILE                     Output File [default: output.txt].
  -i <file>, --input <file>                 Input File.
  -m <message>, --message <input>           Input message [plaintext or ciphertext].
  --aes <AES>                                AES Types [default: AES 128] AES 128, AES 192, AES 256.
  --mode <mode>                             Input Mode [default: ECB] ECB, CBC, CFB.
  -p <padding> --padding <padding>          Input Padding. [default: NO_PADDING] NO_PADDING, 1-0-Padding, ...
  --operation <operation>   Input Operation. [default: Encryption] Encryption, Decryption.
  --encryption                              Encryption.
  --decryption                              Decryption.

)";

void parse_user_arguments(const int& argc, const char** argv);

} // namespace aes::arg

#endif //AES_ARGUMENTS_PARSER_HPP
