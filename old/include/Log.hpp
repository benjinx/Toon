#ifndef LOG_HPP
#define LOG_HPP

#include <Config.hpp>
#include <Utils.hpp>
#include <nlohmann/json.hpp>

#include <cstdio> // for printf, vsnprintf

#if defined(WIN32)
#include <windows.h>
#endif

/// Start Benchmark

#include <chrono>

#define BenchStart() \
    auto benchClockStart = std::chrono::high_resolution_clock::now();

#define BenchEnd(funcName)                                                        \
    LogPerf("Function: %s took %.3f millis\n", funcName,                    \
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(        \
            std::chrono::high_resolution_clock::now() - benchClockStart    \
        ).count());

/// End Benchmark

enum LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_PERF,
    LOG_VERBOSE,
    LOG_LOAD,
};


template <class T>
static auto LogWrap(const T& v) {
    return v;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

template <> 
auto LogWrap<std::string>(const std::string& v) {
    return v.c_str();
}

template <>
inline auto LogWrap<nlohmann::json>(const nlohmann::json& v) {
    static std::string str;
    str = v.get<std::string>();
    return str.c_str();
}
    
#pragma clang diagnostic pop

template <class ...Args>
static inline void Log(LogLevel level, const char * format, Args... args)
{
    #if defined(WIN32)

    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const int DEFAULT = 7;

    int color = DEFAULT;

    switch (level)
    {
    case LOG_INFO:
        color = 7; // White
        break;
    case LOG_WARN:
        color = 6; // Yellow
        break;
    case LOG_ERROR:
        color = 4; // Red
        break;
    case LOG_PERF:
        color = 5; // Magenta
        break;
    case LOG_VERBOSE:
        color = 8; // Grey
        break;
    case LOG_LOAD:
        color = 2; // Green
        break;
    }

    SetConsoleTextAttribute(hConsole, color);

    #else 
    const short FG_DEFAULT = 39;
    const short BG_DEFAULT = 49;

    short fgColor = FG_DEFAULT;
    short bgColor = BG_DEFAULT;

    switch (level)
    {
    case LOG_INFO:
        fgColor = 97; // White
        break;
    case LOG_WARN:
        fgColor = 33; // Yellow
        break;
    case LOG_ERROR:
        fgColor = 31; // Red
        break;
    case LOG_PERF:
        fgColor = 35; // Magenta
        break;
    case LOG_VERBOSE:
        fgColor = 37; // Grey
        break;
    case LOG_LOAD:
        fgColor = 32; // Green
        break;
    }

    printf("\033[%dm\033[%dm", fgColor, bgColor);

    #endif

    printf(format, LogWrap(args)...);

    #if defined(WIN32)
    
    SetConsoleTextAttribute(hConsole, DEFAULT);

    #else

    printf("\033[%dm\033[%dm", FG_DEFAULT, BG_DEFAULT);

    #endif
}

#ifndef TEMPORALITY_ENABLE_VERBOSE_LOGGING
#   define LogVerbose(M, ...)  do { } while(0)
#else
#   define LogVerbose(M, ...) \
        do { Log(LogLevel::LOG_VERBOSE, "[VERB](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)
#endif

#define LogInfo(M, ...) \
    do { Log(LogLevel::LOG_INFO, "[INFO](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { Log(LogLevel::LOG_WARN, "[WARN](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { Log(LogLevel::LOG_ERROR, "[ERRO](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogPerf(M, ...) \
    do { Log(LogLevel::LOG_PERF, "[PERF](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#define LogLoad(M, ...) \
    do { Log(LogLevel::LOG_LOAD, "[LOAD](%s:%d) " M "\n", Utils::GetBasename(__FILE__).c_str(), __LINE__, ##__VA_ARGS__); } while (0)

#endif //LOG_HPP