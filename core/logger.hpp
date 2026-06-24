#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>

class Logger
{
private:
    inline static std::mutex mutex;

public:
    enum class Level
    {
        INFO,
        WARN,
        ERROR,
        DEBUG
    };

    static void log(
        Level level,
        const std::string &tag,
        const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mutex);

        auto now =
            std::chrono::system_clock::now();

        auto time =
            std::chrono::system_clock::to_time_t(now);

        std::cout
            << "["
            << std::put_time(
                   std::localtime(&time),
                   "%H:%M:%S")
            << "] "
            << "["
            << level_to_string(level)
            << "] "
            << "["
            << tag
            << "] "
            << message
            << '\n';
    }

private:
    static const char *level_to_string(Level level)
    {
        switch (level)
        {
        case Level::INFO:
            return "INFO";

        case Level::WARN:
            return "WARN";

        case Level::ERROR:
            return "ERROR";

        case Level::DEBUG:
            return "DEBUG";
        }

        return "UNKNOWN";
    }
};

#define LOG_INFO(tag, msg) Logger::log(Logger::Level::INFO, tag, msg)

#define LOG_WARN(tag, msg) Logger::log(Logger::Level::WARN, tag, msg)

#define LOG_ERROR(tag, msg) Logger::log(Logger::Level::ERROR, tag, msg)

#define LOG_DEBUG(tag, msg) Logger::log(Logger::Level::DEBUG, tag, msg)