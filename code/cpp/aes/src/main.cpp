#include <cstdlib>
#include "arguments_parser/arguments_parser.hpp"

#define RELEASE_MODE //TODO: uncomment/comment when in release mode.

int main(const int argc, const char** argv)
{

#ifndef RELEASE_MODE
    aes::log::Logger::init();
#endif

    aes::arg::parse_user_arguments(argc, argv);

#ifndef RELEASE_MODE
    aes::log::Logger::shutdown();
#endif

    return EXIT_SUCCESS;
}
