#include "Logger.hpp"

bool Logger::Init()
{
    spdlog::set_level(spdlog::level::debug);
    LOG_INFO("Logger initialization successful.");
    return true;
}
