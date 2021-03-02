#include <Toon/Util.hpp>
#include <Toon/Log.hpp>

#include <sstream>

namespace Toon {

TOON_ENGINE_API
string GetDirname(string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == string::npos
        ? path
        : path.substr(0, pivot)
    );
}

TOON_ENGINE_API
string GetBasename(string path)
{
    size_t pivot = path.find_last_of('/');
    if (pivot == string::npos) {
        pivot = path.find_last_of('\\');
    }
    return (
        pivot == string::npos
        ? path
        : path.substr(pivot + 1)
    );
}

TOON_ENGINE_API
string GetExtension(string path)
{
    size_t pivot = path.find_last_of('.');
    return (
        pivot == string::npos
        ? string()
        : path.substr(pivot + 1)
    );
}

TOON_ENGINE_API
std::vector<Path> GetAssetPathList()
{
    static std::vector<Path> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back(Path("Assets"));

    const char * path = getenv("TOON_ASSET_PATH");
    ToonLogVerbose("TOON_ASSET_PATH=%s", path);
    
    if (path) {
        std::istringstream iss(path);
        string p;
        while (std::getline(iss, p, TOON_PATH_SEPARATOR)) {
            paths.push_back(Path(p));
        }
    }
    
    return paths;
}

#if defined(TOON_PLATFORM_WINDOWS)

TOON_ENGINE_API
std::wstring ConvertUTF8ToWideString(string str)
{
    size_t maxSize = str.size() + 1;

    // Initialize to maximum potential size
    std::vector<wchar_t> wide(maxSize);

    int result = MultiByteToWideChar(
        CP_UTF8, 0, 
        str.c_str(), -1, 
        wide.data(), wide.size());
    
    if (result <= 0) {
        return std::wstring();
    }

    return std::wstring(wide.data());
}

TOON_ENGINE_API
string ConvertWideStringToUTF8(std::wstring wstr)
{
    size_t maxSize = (wstr.size() * 4) + 1;

    // Initialize to maximum potential size
    std::vector<char> utf8(maxSize);

    int result = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.c_str(), -1,
        utf8.data(), utf8.size(), 
        NULL, NULL);
        
    if (result <= 0) {
        return string();
    }

    return string(utf8.data());
}

#endif // defined(TOON_PLATFORM_WINDOWS)

} // namespace Toon