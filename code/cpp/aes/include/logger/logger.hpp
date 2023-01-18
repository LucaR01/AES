//
// Created by Luca on 07/12/2022.
//

#ifndef AES_LOGGER_H
#define AES_LOGGER_H

#include <source_location> //TODO: usare source_location::current(); .file(); .file_name(); .function_name();

#include "spdlog/spdlog.h"
#include "os/os.hpp"

namespace aes::log {

//TODO: se non uso il private, potrei fare una struct al posto della classe.
//TODO: potrei fare un Singleton.
//TODO: forse non mi serve nè una struct nè una classe.
class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    static void init();
    static void shutdown();
};

}

#define LOGGER_NAME "AES_LOGGER"

//#define RELEASE_MODE //TODO: uncomment when running in release mode.

//TODO: volendo usare __FUNCTION__ (fa la stessa cosa di __func__) e __PRETTY_FUNCTION__ (mostra return type e parameter types) ma non sono standard.

#ifndef RELEASE_MODE
#define AES_TRACE(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->trace(__VA_ARGS__) : void();
#define AES_DEBUG(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->debug(__VA_ARGS__) : void();
#define AES_INFO(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->info(__VA_ARGS__) : void();
#define AES_WARN(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->warn(__VA_ARGS__) : void();
#define AES_ERROR(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->error(__VA_ARGS__) : void();
#define AES_CRITICAL(...) spdlog::get(LOGGER_NAME) ? spdlog::get(LOGGER_NAME)->critical(__VA_ARGS__) : void();

#define AES_ASSERT(condition, message) if(condition) {} else { AES_CRITICAL("ASSERT | {}\n\t{}\n\t in file: {}\n\t function: {}\n\t line: {}", #condition, \
message, __FILE__, __func__, __LINE__); }
#define AES_ASSERT_BREAK(condition, message) if(condition) {} else { AES_CRITICAL("ASSERT | {}\n\t{}\n\t in file: {}\n\t function: {}\n\t line: {}", #condition, \
message, __FILE__, __func__, __LINE__); AES_BREAK; }
#else
#define AES_TRACE(...)
#define AES_DEBUG(...)
#define AES_INFO(...)
#define AES_WARN(...)
#define AES_ERROR(...)
#define AES_CRITICAL(...)

#define AES_ASSERT(condition, message)
#define AES_ASSERT_BREAK(condition, message)
#endif

#endif //AES_LOGGER_H
