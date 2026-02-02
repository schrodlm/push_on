#pragma once
#include <iostream>
#include <string>
#include <sstream>

// Simple logging system with different levels
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static void SetLevel(LogLevel level) {
        s_minLevel = level;
    }

    static void Debug(const std::string& message) {
        Log(LogLevel::DEBUG, message);
    }

    static void Info(const std::string& message) {
        Log(LogLevel::INFO, message);
    }

    static void Warning(const std::string& message) {
        Log(LogLevel::WARNING, message);
    }

    static void Error(const std::string& message) {
        Log(LogLevel::ERROR, message);
    }

    // Convenience method for formatted messages
    template<typename... Args>
    static void Debug(Args&&... args) {
        if (s_minLevel <= LogLevel::DEBUG) {
            std::ostringstream oss;
            ((oss << args), ...);
            Log(LogLevel::DEBUG, oss.str());
        }
    }

    template<typename... Args>
    static void Info(Args&&... args) {
        if (s_minLevel <= LogLevel::INFO) {
            std::ostringstream oss;
            ((oss << args), ...);
            Log(LogLevel::INFO, oss.str());
        }
    }

    template<typename... Args>
    static void Warning(Args&&... args) {
        if (s_minLevel <= LogLevel::WARNING) {
            std::ostringstream oss;
            ((oss << args), ...);
            Log(LogLevel::WARNING, oss.str());
        }
    }

    template<typename... Args>
    static void Error(Args&&... args) {
        if (s_minLevel <= LogLevel::ERROR) {
            std::ostringstream oss;
            ((oss << args), ...);
            Log(LogLevel::ERROR, oss.str());
        }
    }

private:
    static LogLevel s_minLevel;

    static void Log(LogLevel level, const std::string& message) {
        if (level < s_minLevel) return;

        std::string prefix;
        switch (level) {
            case LogLevel::DEBUG:   prefix = "[DEBUG] "; break;
            case LogLevel::INFO:    prefix = "[INFO]  "; break;
            case LogLevel::WARNING: prefix = "[WARN]  "; break;
            case LogLevel::ERROR:   prefix = "[ERROR] "; break;
        }

        std::cout << prefix << message << std::endl;
    }
};

// Initialize static member
inline LogLevel Logger::s_minLevel = LogLevel::DEBUG;
