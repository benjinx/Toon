#ifndef TOON_LOG_HPP
#define TOON_LOG_HPP

#include <Toon/Utils.hpp>
#include <Toon/Macros.hpp>

#include <cstdio> // for printf, vsnprintf

#if !defined(TOON_PLATFORM_WINDOWS)

    #include <unistd.h>

#endif

/// Start Benchmark

#include <chrono>

#define BenchmarkStart() \
    auto toonBenchClockStart = std::chrono::high_resolution_clock::now();

#define BenchmarkEnd(funcName)                                                      \
    LogPerf("Function: %s took %.3f millis\n", TOON_FUNCTION_NAME(),         \
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(      \
            std::chrono::high_resolution_clock::now() - toonBenchClockStart  \
        ).count());

/// End Benchmark

namespace Toon {

    enum class LogLevel {
        Fatal,
        Error,
        Warning,
        Info,
        Performance,
        Load,
        Verbose,
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

#pragma clang diagnostic pop

template <class ...Args>
static inline void Log(LogLevel level, const char* format, Args... args)
{
    #if defined(TOON_PLATFORM_WINDOWS)

        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        const int DEFAULT = 7;

        int color = DEFAULT;

        switch (level)
        {
        case LogLevel::Info:
            color = 7; // White
            break;
        case LogLevel::Warning:
            color = 6; // Yellow
            break;
        case LogLevel::Error:
        case LogLevel::Fatal:
            color = 4; // Red
            break;
        case LogLevel::Performance:
            color = 5; // Magenta
            break;
        case LogLevel::Verbose:
            color = 8; // Grey
            break;
        case LogLevel::Load:
            color = 2; // Green
            break;
        }

        SetConsoleTextAttribute(hConsole, color);

    #else
    
        static bool isTTY = isatty(fileno(stdout));

        const char * Default = 0;39;

        const char * color = Default;

        switch (level)
        {
        case LogLevel::Info:
            color = "0;97"; // White
            break;
        case LogLevel::Warning:
            color = "0;33"; // Yellow
            break;
        case LogLevel::Error:
        case LogLevel::Fatal:
            color = "0;31"; // Red
            break;
        case LogLevel::Performance:
            color = "0;35"; // Magenta
            break;
        case LogLevel::Verbose:
            color = "1;37"; // Grey
            break;
        case LogLevel::Load:
            color = "0;32"; // Green
            break;
        }

        if (isTTY) {
            printf("\033[%sm", color);
        }


    #endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
        
    printf(format, LogWrap(args)...);

#pragma clang diagnostic pop

#pragma GCC diagnostic pop

#if defined(TOON_PLATFORM_WINDOWS)

        SetConsoleTextAttribute(hConsole, DEFAULT);

#else

        if (isTTY) {
            printf("\033[%sm", Default);
        }

#endif
}

#if !defined(TOON_SOURCE_PATH_LENGTH)
    #define TOON_SOURCE_PATH_LENGTH 0
#endif

#define TOON_FILENAME (&__FILE__[TOON_SOURCE_PATH_LENGTH])

#ifndef TOON_ENABLE_VERBOSE_LOGGING
#   define LogVerbose(M, ...)  do { } while(0)
#else
#   define LogVerbose(M, ...) \
        do { Log(Toon::LogLevel::Verbose, "[VERB](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)
#endif

#define LogInfo(M, ...) \
    do { Log(Toon::LogLevel::Info, "[INFO](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { Log(Toon::LogLevel::Warning, "[WARN](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { Log(Toon::LogLevel::Error, "[ERRO](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define LogFatal(M, ...) \
    do { Log(Toon::LogLevel::Fatal, "[FATL](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); \
        std::terminate(); } while (0)

#define LogPerf(M, ...) \
    do { Log(Toon::LogLevel::Performance, "[PERF](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

#define LogLoad(M, ...) \
    do { Log(Toon::LogLevel::Load, "[LOAD](%s:%d) " M "\n", \
        TOON_FILENAME, __LINE__, ##__VA_ARGS__); } while (0)

} // namespace Toon
#endif // TOON_LOG_HPP