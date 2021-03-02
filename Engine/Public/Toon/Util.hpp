#ifndef TOON_UTIL_HPP
#define TOON_UTIL_HPP

#include <Toon/Config.hpp>
#include <Toon/Platform.hpp>
#include <Toon/Path.hpp>

#include <Toon/String.hpp>
#include <vector>

namespace Toon {

TOON_ENGINE_API
string GetDirname(string path);

TOON_ENGINE_API
string GetBasename(string path);

TOON_ENGINE_API
string GetExtension(string path);

TOON_ENGINE_API
std::vector<Path> GetAssetPathList();

#if defined(TOON_PLATFORM_WINDOWS)

TOON_ENGINE_API
std::wstring ConvertUTF8ToWideString(string str);

TOON_ENGINE_API
string ConvertWideStringToUTF8(std::wstring str);

class WindowsErrorMessage
{
public:

    WindowsErrorMessage(HRESULT hResult) {
        FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hResult,
            GetUserDefaultUILanguage(),
            (LPTSTR)&_message,
            0,
            nullptr
        );

    };

    ~WindowsErrorMessage() {
        if (_message) {
            LocalFree(_message);
            _message = nullptr;
        }
    }

    char * GetMessage() const {
        return _message;
    }

private:

    char * _message = nullptr;

};

#endif // defined(TOON_PLATFORM_WINDOWS)

} // namespace Toon

#endif // TOON_UTIL_HPP