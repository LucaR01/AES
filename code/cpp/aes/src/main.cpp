#include <iostream>

#include "logger/logger.hpp"
#include "graphics/console/console.hpp"

int main() {

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

    aes::con::show_console(); //TODO: remove

#ifndef RELEASE_MODE
    //aes_logger.shutdown();
    aes::log::Logger::shutdown();
#endif

    return 0;
}
