#ifndef TOON_VERSION_HPP
#define TOON_VERSION_HPP

#include <Toon/Config.hpp>

#include <Toon/String.hpp>
#include <regex>

namespace Toon {

struct TOON_ENGINE_API Version
{
    int Major;
    int Minor;
    int Patch;

    Version(int major = 0, int minor = 0, int patch = 0)
        : Major(major)
        , Minor(minor)
        , Patch(patch)
    { }

    Version(const Version& rhs)
        : Major(rhs.Major)
        , Minor(rhs.Minor)
        , Patch(rhs.Patch)
    { }

    Version(const string& str)
    {
        FromString(str);
    }

    inline Version& operator=(const Version& rhs)
    {
        Major = rhs.Major;
        Minor = rhs.Minor;
        Patch = rhs.Patch;
        return *this;
    }

    inline bool operator==(const Version& rhs)
    {
        return (Compare(*this, rhs) == 0);
    }

    inline bool operator!=(const Version& rhs)
    {
        return !(*this == rhs);
    }

    inline friend bool operator>(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == 1);
    }

    inline friend bool operator>=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == 1);
    }

    inline friend bool operator<(const Version& lhs, const Version& rhs)
    {
        return (Version::Compare(lhs, rhs) == -1);
    }

    inline friend bool operator<=(const Version& lhs, const Version& rhs)
    {
        int res = Version::Compare(lhs, rhs);
        return (res == 0 || res == -1);
    }

    inline string ToString() const
    {
        return std::to_string(Major) +
            "." +
            std::to_string(Minor) +
            "." +
            std::to_string(Patch);
    }

    inline operator string() const
    {
        return ToString();
    }

    inline void FromString(const string& string)
    {
        std::smatch match;
        std::regex regex("^.*?([0-9])+\\.([0-9]+)\\.([0-9]+).*");
        std::regex_match(string, match, regex);

        if (match.size() == 4) {
            Major = std::strtol(match[1].str().c_str(), nullptr, 10);
            Minor = std::strtol(match[2].str().c_str(), nullptr, 10);
            Patch = std::strtol(match[3].str().c_str(), nullptr, 10);
        }
    }

    inline static int Compare(const Version& a, const Version& b)
    {
        auto cmp = [](int a, int b) {
            if (a == b) {
                return 0;
            }
            if (a > b) {
                return 1;
            }
            return -1;
        };

        int result = cmp(a.Major, b.Major);
        if (result == 0) {
            result = cmp(a.Minor, b.Minor);
            if (result == 0) {
                result = cmp(a.Patch, b.Patch);
            }
        }
        return result;
    }
}; // struct Version

} // namespace Toon

#endif // TOON_VERSION_HPP