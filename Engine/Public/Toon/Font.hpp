#ifndef TOON_FONT_HPP
#define TOON_FONT_HPP

#include <Toon/Config.hpp>

#include <Toon/String.hpp>

namespace Toon {

class TOON_ENGINE_API Font
{
public:

    DISALLOW_COPY_AND_ASSIGN(Font)

    bool LoadFromFile(const string& filename);

}; // class Font

}; // namespace Toon

#endif // TOON_FONT_HPP