#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <memory>

//#ifndef RELEASE_MODE //TODO: così forse dà errore perché poi non vede i AES_TRACE, ecc.
#include "logger/logger.hpp"
//#endif

#include "core/aes_api.hpp"

#include "graphics/console/console.hpp"
#include "graphics/gui/gui.hpp"
#include "file_manager/file_manager.hpp" //TODO: remove?
//#include "core/modes/modes.hpp"
//#include "core/padding/padding.hpp"
#include "version/version.hpp"
#include "arguments_parser/arguments_parser.hpp"

#include "docopt.h"

//TODO: dove uso constexpr, metterli nell'.hpp; tipo negli enums.

int main(const int argc, const char** argv)
{

#ifndef RELEASE_MODE
    aes::log::Logger::init();
#endif

    //TODO: remove, creare un test a riguardo.
    /*std::vector<std::string> data = { "hello ", " everyone ", "! " };
    aes::fm::FileManager::write_file_data("test.txt", data);
    AES_DEBUG("Filename: {} exists: {}", aes::fm::FileManager::get_filename("test.txt"), aes::fm::FileManager::file_exists("test.txt"))
    std::vector<char*> data2 = aes::fm::FileManager::get_file_data("test.txt");

    for(const auto& d : data2) {
        std::cout << d << std::endl;
    }

    std::vector<std::string> data3 = aes::fm::FileManager::get_file_data2("test.txt");

    for(const auto& d : data3) {
        std::cout << d << std::endl;
    }

    std::cout << "get_key(test.txt): " << aes::fm::FileManager::get_key("test.txt") << std::endl;

    aes::fm::FileManager::delete_file("test.txt");

    AES_DEBUG("Filename: {} exists: {}", aes::fm::FileManager::get_filename("test.txt"), aes::fm::FileManager::file_exists("test.txt"))*/

    //aes::con::show_console(); //TODO: remove/testare

    //TODO: da mettere in un test.
    //std::cout << "TEST 0 " << std::endl;
    /*std::vector<unsigned char> plain = { 0x80, 0x11, 0x22, 0x13, 0x44, 0x12, 0x66, 0x77, 0x88, 0x99, 0xFF, 0xBB, 0xAA, 0x22, 0x33, 0x24 };
    std::vector<unsigned char> key = { 0x02, 0x04, 0x12, 0x09, 0x04, 0x09, 0xBB, 0x06, 0x08, 0xDD, 0x0a, 0x0b, 0x0A, 0x0B, 0x0E, 0x0F };

    std::cout << "plain text: ";
    for(const auto& p : plain) {
        std::cout << p;
    }

    std::cout << " " << std::endl;

    std::string plain_text{plain.begin(), plain.end()};
    std::cout << "plain_text in string: " << plain_text << std::endl;

    auto ciphertext = aes::mod::encrypt_ECB(plain, key, aes::AES::AES_128);
    //std::cout.setf(std::ios::hex);
    std::cout << "ciphertext: " << ciphertext << std::endl;
    //std::cout.unsetf(std::ios::hex);

    std::vector<unsigned char> out(*ciphertext);
    std::vector<unsigned char> out2(ciphertext, ciphertext + 0);

    auto deciphertext = aes::mod::decrypt_ECB(out, key, aes::AES::AES_128);
    std::cout << "deciphertext: " << deciphertext << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "TEST 1 - TESTO CIFRATO COMBACIA" << std::endl;
    std::vector<unsigned char> plain2 = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    std::vector<unsigned char> key2 = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    std::vector<unsigned char> right_output = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

    std::string plain_text2{plain2.cbegin(), plain2.cend()};
    std::cout << "plain_text2: " << plain_text2 << std::endl;

    auto ciphertext2 = aes::mod::encrypt_ECB(plain2, key2, aes::AES::AES_128);
    std::cout << "ciphertext2: " << ciphertext2 << std::endl;
    //std::cout << "right_output: " << std::hex << right_output << std::endl; //TODO: non funziona
    std::cout << "right_output: ";
    for(const auto& o : right_output) {
        //std::cout.setf(std::ios::hex);
        std::cout << o;
    }

    //std::cout.unsetf(std::ios::hex);

    std::cout << "" << std::endl;

    auto deciphertext2 = aes::mod::decrypt_ECB(std::vector<unsigned char>(*ciphertext2), key2, aes::AES::AES_128);
    std::cout << "deciphertext2: " << deciphertext2 << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "TEST 2" << std::endl;
    std::string_view testo = "Helloworld!hello"; // 16 caratteri
    std::vector<unsigned char> testo_in_vector(testo.cbegin(), testo.cend());

    std::cout << "testo_in_vector: ";
    for(const auto& t : testo_in_vector) {
        std::cout << t;
    }

    std::cout << "" << std::endl;

    auto ciphertext3 = aes::mod::encrypt_ECB(testo_in_vector, key2, aes::AES::AES_128);
    auto deciphertext3 = aes::mod::decrypt_ECB(std::vector<unsigned char>(*ciphertext3), key2, aes::AES::AES_128);

    std::cout << "ciphertext3: " << ciphertext3 << std::endl;
    std::cout << "deciphertext3: " << deciphertext3 << std::endl;*/

    //TODO: c++20 std::to_array(c array);

    //TODO: uncomment; funziona; da mettere nei tests.

    /*unsigned char plain4[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                             0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    unsigned char key4[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char right4[] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
                             0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

    unsigned char* out4 = aes::mod::encrypt_ECB(plain4, 16 * sizeof(unsigned char), key4, aes::AES::AES_128);

    std::cout << "test encryption corretta" << std::endl;
    std::cout << "plain4: " << plain4 << std::endl;
    std::cout << "key4: " << key4 << std::endl;
    std::cout << "right4: " << right4 << std::endl;
    std::cout << "out4: " << out4 << std::endl;

    delete[] out4;*/

    // -----------------------------------------------------------------------------------------------------------------

    /*unsigned char encrypted[] = {0x69, 0xc4, 0xe0, 0xd8, 0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b,
                                 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80,0x70, 0xb4, 0xc5, 0x5a};
    unsigned char key5[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                            0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char right5[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                              0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,0xcc, 0xdd, 0xee, 0xff};

    unsigned char *out5 = aes::mod::decrypt_ECB(encrypted, 20 * sizeof(unsigned char), key5, aes::AES::AES_128);

    std::cout << "test decryption corretta" << std::endl;
    std::cout << "encrypted: " << encrypted << std::endl;
    std::cout << "key5: " << key5 << std::endl;
    std::cout << "right5: " << right5 << std::endl;
    std::cout << "out5: " << out5 << std::endl;

    std::cout << "right5: " << std::endl;
    for(unsigned char r : right5) {
        std::cout << "0x" << std::hex << (int)r << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "out5: " << std::endl;
    for(unsigned short i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << (int)out5[i] << ", ";
    }

    delete[] out5;*/

    // -----------------------------------------------------------------------------------------------------------------

    //TODO: uncomment; funziona; da mettere nei tests.
    /*std::vector<unsigned char> encrypted6 = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b,
                                            0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80,
                                            0x70, 0xb4, 0xc5, 0x5a};

    std::vector<unsigned char> key6 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                      0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                      0x0c, 0x0d, 0x0e, 0x0f};

    std::vector<unsigned char> right6 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                        0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
                                        0xcc, 0xdd, 0xee, 0xff};

    //TODO: la decrypt continua a dare risultati diversi ogni volta!
    std::vector<unsigned char> clear_message6 = aes::mod::decrypt_ECB(encrypted6, key6, aes::AES::AES_128);

    std::cout << "encrypted6: " << std::endl;
    for(const auto& e : encrypted6) {
        std::cout << e;
    }

    std::cout << "" << std::endl;

    std::cout << "right6: " << std::endl;
    for(const auto& r : right6) {
        std::cout << r;
    }

    std::cout << "" << std::endl;

    std::cout << "clear_message6: " << std::endl;
    for(const auto& c : clear_message6) {
        std::cout << c;
    }

    std::cout << "" << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    //TODO: questo sotto è sbagliato, è sempre diverso!
    std::string s = "helloeverybodyye";
    std::string key = "key";
    std::vector<unsigned char> ciphertext7 = aes::mod::encrypt_ECB(std::vector<unsigned char>(s.cbegin(), s.cend()), std::vector<unsigned char>(key.cbegin(), key.cend()), aes::AES::AES_128);
    std::vector<unsigned char> deciphertext7 = aes::mod::decrypt_ECB(ciphertext7, std::vector<unsigned char>(key.cbegin(), key.cend()), aes::AES::AES_128);

    std::cout << "stringa: " << std::endl;
    for(const auto& x : std::vector<unsigned char>(s.cbegin(), s.cend())) {
        std::cout << x;
    }

    std::cout << "" << std::endl;

    std::cout << "ciphertext7: " << std::endl;
    for(const auto& c : ciphertext7) {
        std::cout << c;
    }

    std::cout << "" << std::endl;

    std::cout << "deciphertext7: " << std::endl;
    for(const auto& d : deciphertext7) {
        std::cout << d;
    }

    std::cout << "" << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    //TODO: questo sotto è sbagliato, è sempre diverso!
    std::string s2 = "helloeverybodyye";
    std::string key2 = "key";
    uint8_t* ciphertext8 = aes::mod::encrypt_ECB4(std::vector<unsigned char>(s2.cbegin(), s2.cend()), std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);
    uint8_t* deciphertext8 = aes::mod::decrypt_ECB4(std::vector<unsigned char>(ciphertext8, ciphertext8 + 16), std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);

    std::cout << "stringa2: " << std::endl;
    for(const auto& x : std::vector<unsigned char>(s2.cbegin(), s2.cend())) {
        std::cout << x;
    }

    std::cout << "" << std::endl;

    std::cout << "ciphertext8: " << ciphertext8 << std::endl;

    std::cout << "" << std::endl;

    std::cout << "deciphertext8: " << deciphertext8 << std::endl;

    std::cout << "" << std::endl;*/

    // -----------------------------------------------------------------------------------------------------------------

    //TODO: questo sotto è sbagliato, è sempre diverso!
    //TODO: non va!
    /*std::vector<uint8_t> ciphertext9 = aes::mod::encrypt_ECB5(std::vector<unsigned char>(s2.cbegin(), s2.cend()), std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);
    std::vector<uint8_t> deciphertext9 = aes::mod::decrypt_ECB5(ciphertext9, std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);

    std::cout << "stringa2: " << std::endl;
    for(const auto& x : std::vector<unsigned char>(s2.cbegin(), s2.cend())) {
        std::cout << x;
    }

    std::cout << "" << std::endl;

    //std::cout << "ciphertext8: " << ciphertext8 << std::endl;
    std::cout << "ciphertext9: " << std::endl;
    for(const auto& c : ciphertext9) {
        std::cout << c;
    }

    std::cout << "" << std::endl;

    //std::cout << "deciphertext8: " << deciphertext8 << std::endl;
    std::cout << "deciphertext9: " << std::endl;
    for(const auto& d : deciphertext9) {
        std::cout << d;
    }

    std::cout << "" << std::endl;*/

    // -----------------------------------------------------------------------------------------------------------------

    // CBC
    //TODO: uncomment; questo funziona; è da mettere in un test;

    /*std::vector<unsigned char> plain5 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                        0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
                                        0xcc, 0xdd, 0xee, 0xff};

    std::vector<unsigned char> iv = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff};

    std::vector<unsigned char> key3 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                       0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                       0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    std::vector<unsigned char> encryptedCBC = aes::mod::encrypt_CBC(plain5, key3, iv, aes::AES::AES_256);
    std::vector<unsigned char> decryptedCBC = aes::mod::decrypt_CBC(encryptedCBC, key3, iv, aes::AES::AES_256);

    std::cout << "encryptedCBC: " << std::endl;
    for(const auto& e : encryptedCBC) {
        std::cout << e << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "plain5: " << std::endl;
    for(const auto& p : plain5) {
        std::cout << p << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "decryptedCBC: " << std::endl;
    for(const auto& d : decryptedCBC) {
        std::cout << d << ", ";
    }

    std::cout << std::endl;

    std::cout << "----------------------------------------------------------------" << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    // CFB

    std::vector<unsigned char> plain6 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                        0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
                                        0xcc, 0xdd, 0xee, 0xff};

    std::vector<unsigned char> iv2 = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                     0xff, 0xff, 0xff, 0xff};

    std::vector<unsigned char> key5 = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                        0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    std::vector<unsigned char> encryptedCFB = aes::mod::encrypt_CFB(plain6, key5, iv2, aes::AES::AES_256);
    std::vector<unsigned char> decryptedCFB = aes::mod::decrypt_CFB(encryptedCFB, key5, iv2, aes::AES::AES_256);

    std::cout << "encryptedCFB: " << std::endl;
    for(const auto& e : encryptedCFB) {
        std::cout << e << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "plain6: " << std::endl;
    for(const auto& p : plain6) {
        std::cout << p << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "decryptedCFB: " << std::endl;
    for(const auto& d : decryptedCFB) {
        std::cout << d << ", ";
    }

    std::cout << "" << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    // AES::API

    //TODO: uncomment
    std::cout << "--------------------------" << std::endl;
    std::cout << "AES API" << std::endl;

    std::vector<unsigned char> plain7 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
                                         0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb,
                                         0xcc, 0xdd, 0xee, 0xff};

    std::vector<unsigned char> iv3 = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                      0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                      0xff, 0xff, 0xff, 0xff};

    std::vector<unsigned char> key7 = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                                        0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                        0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    std::string plain7_string(plain7.cbegin(), plain7.cend());
    std::string key7_string(key7.cbegin(), key7.cend());
    std::vector<unsigned char> encryptedCFB2 = aes::api::encrypt(plain7_string, key7_string, iv3, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::string encryptedCFB2_string(encryptedCFB2.cbegin(), encryptedCFB2.cend());

    std::vector<unsigned char> decryptedCFB2 = aes::api::decrypt(encryptedCFB2_string, key7_string, iv3, aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::cout << "encryptedCFB2: " << std::endl;
    for(const auto& e : encryptedCFB2) {
        std::cout << e << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "plain7: " << std::endl;
    for(const auto& p : plain7) {
        std::cout << p << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "decryptedCFB2: " << std::endl;
    for(const auto& d : decryptedCFB2) {
        std::cout << d << ", ";
    }

    std::cout << "" << std::endl;

    // -----------------------------------------------------------------------------------------------------------------

    std::cout << "================= CON SOLO STRINGHE TEST =====================" << std::endl;

    std::string plain7_s = "\u0011\u0002 3\u0004@U\u0006`w\b\u0080\u0099\n"" »\fÀÝ\u000Eàÿ";

    std::string iv3_s = "\u000Fðÿ\u000Fðÿ\u000Fðÿ\u000Fðÿ\u000Fðÿ\u000Fðÿ\u000Fðÿ\u000Fðÿ";

    std::string key7_s = "\u0001 \u0003@\u0005`\u0007\u0080\t \u000BÀ\n""à\u000F\u0001\u0011\u0001 \u0013\u0001@\u0015\u0001`\u0017\u0001\u0080\u0019\u0001 \u001B\u0001À\u001D\u0001à\u001F";

    std::vector<unsigned char> encryptedCFB3 = aes::api::encrypt(plain7_s, key7_s, std::vector<uint8_t>(iv3_s.cbegin(), iv3_s.cend()), aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::string encryptedCFB3_string(encryptedCFB3.cbegin(), encryptedCFB3.cend());

    std::vector<unsigned char> decryptedCFB3 = aes::api::decrypt(encryptedCFB3_string, key7_s, std::vector<uint8_t>(iv3_s.cbegin(), iv3_s.cend()), aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::cout << "encryptedCFB3: " << std::endl;
    for(const auto& e : encryptedCFB3) {
        std::cout << e << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "plain7_s: " << std::endl;
    for(const auto& p : plain7_s) {
        std::cout << p << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "decryptedCFB3: " << std::endl;
    for(const auto& d : decryptedCFB3) {
        std::cout << d << ", ";
    }

    std::cout << "" << std::endl;*/

    // -----------------------------------------------------------------------------------------------------------------

    /*std::cout << "================= STRINGHE INPUT TEST =====================" << std::endl; //TODO: uncomment

    std::cout << "Insert plaintext: " << std::endl;
    std::string plain8_s;
    std::cin.ignore();
    std::getline(std::cin, plain8_s);

    AES_DEBUG("plain8_s: {}", plain8_s)

    std::cout << "Insert iv: " << std::endl;
    std::string iv4_s;
    std::cin.ignore();
    std::getline(std::cin, iv4_s);

    AES_DEBUG("iv4_s: {}", iv4_s)

    std::cout << "Insert key: " << std::endl;
    std::string key8_s;
    std::cin.ignore();
    std::getline(std::cin, key8_s);

    AES_DEBUG("key8_s: {}", key8_s)

    std::vector<unsigned char> encryptedCFB4 = aes::api::encrypt(plain8_s, key8_s, std::vector<uint8_t>(iv4_s.cbegin(), iv4_s.cend()), aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::string encryptedCFB4_string(encryptedCFB4.cbegin(), encryptedCFB4.cend());

    std::vector<unsigned char> decryptedCFB4 = aes::api::decrypt(encryptedCFB4_string, key8_s, std::vector<uint8_t>(iv4_s.cbegin(), iv4_s.cend()), aes::pad::Paddings::ONE_ZERO_PADDING, aes::mod::Modes::CFB, aes::AES::AES_256);

    std::cout << "encryptedCFB4: " << std::endl;
    for(const auto& e : encryptedCFB4) {
        std::cout << e << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "plain8_s: " << std::endl;
    for(const auto& p : plain8_s) {
        std::cout << p << ", ";
    }

    std::cout << "" << std::endl;

    std::cout << "decryptedCFB4: " << std::endl;
    for(const auto& d : decryptedCFB4) {
        std::cout << d << ", ";
    }

    std::cout << "" << std::endl;*/

    // -----------------------------------------------------------------------------------------------------------------

    //aes::gui::show();

    //TODO: posso templetizzare tutti i get_index dei vari enums da poi mettere in un folder generic_templates o generics o templates

    // -----------------------------------------------------------------------------------------------------------------

    aes::arg::parse_user_arguments(argc, argv);

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
