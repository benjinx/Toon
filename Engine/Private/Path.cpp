#include <Toon/Path.hpp>

#include <algorithm>
#include <cstdio>
#include <climits>
#include <sstream>

#if defined(TOON_PLATFORM_WINDOWS)

    #include <direct.h>

#else

    #include <unistd.h>

#endif

namespace Toon {

TOON_ENGINE_API
Path::Path(const Path& rhs)
    : _path(rhs._path)
{ }

TOON_ENGINE_API
Path::Path(const string& str)
    : _path(str)
{
    Normalize();
}

TOON_ENGINE_API
Path::Path(const char * cstr)
    : _path(cstr)
{
    Normalize();
}

Path& Path::Append(const Path& rhs)
{
    // If path is empty, and doesn't have a trailing separator, append one
    if (rhs._path.empty()) {
        if (!_path.empty() && _path.back() != Separator) {
            _path += Separator;
        }
        return *this;
    }

    if (IsAbsolute() && rhs.IsAbsolute() && GetRootPath() != rhs.GetRootPath()) {
        // Unable to append absolute paths
        _path = rhs._path;
        return *this;
    }


    if (_path.back() != Separator) {
        _path += Separator;
    }

    _path += rhs._path;
    return *this;
}

Path& Path::Concatenate(const Path& path)
{
    _path += path._path;
    return *this;
}

bool Path::Equals(const Path& rhs) const
{
    #if defined(TOON_PLATFORM_WINDOWS)

        return StringEqualCaseInsensitive(_path, rhs._path);

    #else

        return (_path == rhs._path);

    #endif
}

// std::vector<string> Path::GetParts() const
// {
//     std::vector<string> parts;

//     auto it = GetRootNameLength();

//     if (IsAbsolute()) {
//         parts.push_back(string(1, Separator));
//         ++it;
//     }

//     // stringstream ss(_path.substr(it));
//     std::stringstream ss(_path.data() + it);

//     string token;
//     while (getline(ss, token, Separator)) {
//         parts.push_back(token);
//     }

//     return parts;
// }

TOON_ENGINE_API
void Path::Normalize()
{
    if (_path.empty()) {
        return;
    }

    // TODO: Check valid UTF-8

    // TODO: Strip windows long filename marker "\\?\"

    #if defined(TOON_PLATFORM_WINDOWS)

        // Convert slashes to native format
        for (auto& c : _path) {
            if (c == '/') {
                c = '\\';
            }
        }

    #endif

    auto begin = _path.begin();
    auto end = _path.end();

    // Skip double slashes for paths starting like "\\server"
    if (_path.length() >= 2 && _path[0] == Separator && _path[1] == Separator) {
        begin += 2;
    }

    auto newEnd = std::unique(begin, end,
        [](char lhs, char rhs) {
            return (lhs == rhs && lhs == Separator);
        }
    );

    _path.erase(newEnd, end);
}

size_t Path::GetRootNameLength() const
{
    #if defined(TOON_PLATFORM_WINDOWS)

        // Check for windows drive letter, such as "C:"
        if (_path.length() >= 2 && _path[1] == ':') {
            char first = std::toupper(_path[0]);
            if (first >= 'A' && first <= 'Z') {
                return 2;
            }
        }

    #endif

    // Check for network path, such as "//server"
    if (_path.length() >= 3 && _path[0] == Separator && _path[1] == Separator) {
        if (_path[2] != Separator && std::isprint(_path[2])) {
            // Find first separator after server name
            size_t pos = _path.find_first_of(Separator, 3);
            if (pos == string::npos) {
                // The entire path is just a network share name
                return _path.length();
            }
            else {
                return pos;
            }
        }
    }

    // There is no root name
    return 0;
}

Path GetCurrentPath()
{
#if defined(TOON_PLATFORM_WINDOWS)

    std::unique_ptr<char> cwd(_getcwd(nullptr, 0));
    if (cwd) {
        return Path(string(cwd.get()));
    }

#else

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd))) {
        return Path(cwd);
    }

#endif
    
    return Path();
}

} // namespace Toon