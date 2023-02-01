#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <memory>

//#ifndef RELEASE_MODE //TODO: così forse dà errore perché poi non vede i AES_TRACE, ecc.
#include "logger/logger.hpp"
//#endif

#include "graphics/console/console.hpp"
#include "file_manager/file_manager.hpp" //TODO: remove?
#include "core/modes/modes.hpp"

//TODO: dove uso constexpr, metterli nell'.hpp; tipo negli enums.

int main()
{

//TODO: mettere #ifndef #endif fuori dal main? No, non va.
#ifndef RELEASE_MODE
    aes::log::Logger::init();
#endif

    //TODO: remove unnecessary logs
    AES_TRACE("Tracing AES v{}.{}.{}", 1, 0, 0)
    AES_DEBUG("Debugging...")
    AES_INFO("Info.")
    AES_WARN("warn")
    AES_ERROR("error")
    AES_CRITICAL("critical")

    AES_ASSERT(1 == 2, "1 is not equal to 2.")
    //AES_ASSERT_BREAK(1 == 2, "1 is not equal to 2.")

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

    unsigned char plain4[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
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

    delete[] out4;

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

    std::vector<unsigned char> encrypted6 = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b,
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
    /*std::string s2 = "helloeverybodyye";
    std::string key2 = "key";
    std::vector<uint8_t> ciphertext8 = aes::mod::encrypt_ECB3(std::vector<unsigned char>(s2.cbegin(), s2.cend()), std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);
    std::vector<uint8_t> deciphertext8 = aes::mod::decrypt_ECB3(ciphertext8, std::vector<unsigned char>(key2.cbegin(), key2.cend()), aes::AES::AES_128);

    std::cout << "stringa2: " << std::endl;
    for(const auto& x : std::vector<unsigned char>(s2.cbegin(), s2.cend())) {
        std::cout << x;
    }

    std::cout << "" << std::endl;

    std::cout << "ciphertext8: " << std::endl;
    for(const auto& c : ciphertext8) {
        std::cout << c;
    }

    std::cout << "" << std::endl;

    std::cout << "deciphertext8: " << std::endl;
    for(const auto& d : deciphertext8) {
        std::cout << d;
    }

    std::cout << "" << std::endl;*/

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
