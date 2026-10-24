#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/spdlog.h>

#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)

class Logger {
public:
    static Logger& GetInstance()
    {
        static Logger logger;
        return logger;
    }

    bool Init();
};

#endif