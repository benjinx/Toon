#ifndef TOON_STRING_HPP
#define TOON_STRING_HPP

#include <Toon/Config.hpp>

#include <string>
#include <algorithm>

namespace Toon {

using std::string;
using std::string_view;

inline bool StringEqualCaseInsensitive(const string& a, const string& b) {
    return std::equal(
        a.begin(), a.end(),
        b.begin(), b.end(),
        [](char a, char b) {
            return tolower(a) == tolower(b);
        }
    );
}

} // namespace Toon

#endif // TOON_STRING_HPP