#ifndef TOON_PATH_HPP
#define TOON_PATH_HPP

#include <Toon/Config.hpp>
#include <Toon/String.hpp>

#include <vector>

namespace Toon {

// class PathIterator;

class TOON_ENGINE_API Path
{
public:

    // typedef PathIterator Iterator;
    // typedef PathIterator ConstIterator;

    static const char Separator = TOON_PATH_SLASH;

    Path() = default;

    Path(const Path& rhs);

    Path(const string& str);

    Path(const char * cstr);

    // Appends a new element to the path with a separator
    Path& Append(const Path& rhs);

    // Concatenates the paths without adding a separator
    Path& Concatenate(const Path& rhs);

    inline string ToString() const {
        return _path;
    }

    inline const char * ToCString() const {
        return _path.c_str();
    }

    inline operator string() const {
        return _path;
    }

    inline bool IsAbsolute() const {
        return HasRootDirectory();
    }

    inline bool IsRelative() const {
        return !HasRootDirectory();
    }

    inline bool HasRootName() const {
        return (GetRootNameLength() > 0);
    }

    /// The root identifier on a filesystem with multiple roots, such as "C:" or "//server"
    inline Path GetRootName() const {
        return _path.substr(0, GetRootNameLength());
    }

    inline bool HasRootDirectory() const {
        size_t rootNameLen = GetRootNameLength();
        return (_path.length() > rootNameLen && _path[rootNameLen] == Separator);
    }

    inline Path GetRootDirectory() const {
        if (HasRootDirectory()) {
            return string(1, Separator);
        }
        return Path();
    }

    inline bool HasRootPath() const {
        return (HasRootName() || HasRootDirectory());
    }

    inline Path GetRootPath() const {
        return GetRootName() + GetRootDirectory();
    }

    inline Path GetParentPath() const {
        size_t pivot = _path.find_last_of(Separator);
        return (
            pivot <= GetRootNameLength() || pivot == string::npos
            ? _path
            : _path.substr(0, pivot)
        );
    }

    inline Path GetFilename() const {
        size_t pivot = _path.find_last_of(Separator);
        return (
            pivot <= GetRootNameLength() || pivot == string::npos
            ? _path
            : _path.substr(pivot + 1)
        );
    }

    inline Path GetStem() const {
        string filename = GetFilename();
        size_t pivot = filename.find_last_of('.');
        return (
            pivot == 0 || pivot == string::npos
            ? filename
            : filename.substr(0, pivot)
        );
    }

    inline Path GetExtension() const {
        string filename = GetFilename();
        size_t pivot = filename.find_last_of('.');
        return (
            pivot == 0 || pivot == string::npos
            ? string()
            : filename.substr(pivot + 1)
        );
    }

    inline Path& operator=(const Path& rhs) {
        _path = rhs._path;
        return *this;
    }

    inline Path& operator/=(const Path& rhs) {
        return Append(rhs);
    }

    inline Path& operator+=(const Path& rhs) {
        return Concatenate(rhs);
    }

    bool Equals(const Path& rhs) const;

    inline friend bool operator==(const Path& lhs, const Path& rhs) {
        return lhs.Equals(rhs);
    }

    inline friend bool operator!=(const Path& lhs, const Path& rhs) {
        return !lhs.Equals(rhs);
    }

    inline friend Path operator/(const Path& lhs, const Path& rhs)
    {
        Path tmp(lhs);
        tmp /= rhs;
        return tmp;
    }

    inline friend Path operator+(const Path& lhs, const Path& rhs)
    {
        Path tmp(lhs);
        tmp += rhs;
        return tmp;
    }

    // std::vector<string> GetParts() const;

    // Iterator begin();

    // Iterator end();

    // ConstIterator begin() const;

    // ConstIterator end() const;

private:

    void Normalize();

    size_t GetRootNameLength() const;

    string _path;

}; // class Path

// class TOON_ENGINE_API Path::Iterator
// {
// public:

    

// }; // class Iterator

TOON_ENGINE_API
Path GetCurrentPath();

} // namespace Toon

#endif // TOON_PATH_HPP