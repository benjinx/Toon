#ifndef TEMPORALITY_VERSION_HPP
#define TEMPORALITY_VERSION_HPP

#include <Temporality/Config.hpp>

#include <string>

namespace Temporality {

struct TEMPORALITY_ENGINE_API Version
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

    inline std::string GetString() const
    {
        return std::to_string(Major) +
            "." +
            std::to_string(Minor) +
            "." +
            std::to_string(Patch);
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

} // namespace Temporality

#endif // TEMPORALITY_VERSION_HPP