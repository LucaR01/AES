//
// Created by Luca on 07/12/2022.
//

#include <memory>
#include <vector>

#include "logger/logger.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace aes::log {

void Logger::init()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%l] [%d-%m-%Y %T.%e] %v%$");

    std::vector<spdlog::sink_ptr> sinks{ console_sink };
    auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(logger);
}

void Logger::shutdown()
{
    spdlog::shutdown();
}

} // namespace aes::log
