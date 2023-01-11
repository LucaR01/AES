#include <iostream>
#include <cstdlib>

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
    /*std::vector<unsigned char> plain = { 0x80, 0x11, 0x22, 0x13, 0x44, 0x12, 0x66, 0x77, 0x88, 0x99, 0xFF, 0xBB, 0xAA, 0x22, 0x33, 0x24 };
    std::vector<unsigned char> key = { 0x02, 0x04, 0x12, 0x09, 0x04, 0x09, 0xBB, 0x06, 0x08, 0xDD, 0x0a, 0x0b, 0x0A, 0x0B, 0x0E, 0x0F };

    auto ciphertext = aes::mod::encrypt_ECB(plain, key, aes::AES::AES_128);
    std::cout << "ciphertext: " << ciphertext << std::endl;*/

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
