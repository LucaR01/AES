#include <iostream>

#include "logger/logger.hpp"
#include "graphics/console/console.hpp"
#include "file_manager/file_manager.hpp" //TODO: remove?

//TODO: dove uso constexpr, metterli nell'.hpp; tipo negli enums.

int main() {

//TODO: mettere #ifndef #endif fuori dal main?
#ifndef RELEASE_MODE
    //aes::log::Logger aes_logger;
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
    std::vector<std::string> data = { "hello ", " everyone ", "! " };
    aes::fm::FileManager::get_instance().write_file_data("test.txt", data);
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

    AES_DEBUG("Filename: {} exists: {}", aes::fm::FileManager::get_filename("test.txt"), aes::fm::FileManager::file_exists("test.txt"))

    aes::con::show_console(); //TODO: remove/testare

#ifndef RELEASE_MODE
    //aes_logger.shutdown();
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
