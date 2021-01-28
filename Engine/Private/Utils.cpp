#include <Toon/Utils.hpp>
#include <Toon/Log.hpp>

#include <sstream>

namespace Toon {

TOON_ENGINE_API
std::vector<std::string> GetAssetPaths()
{
    static std::vector<std::string> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back(std::string());

    const char * path = getenv("TOON_ASSET_PATH");
    LogVerbose("TOON_ASSET_PATH=%s", path);

    if (path) {
        std::istringstream iss(path);
        std::string p;
        while (std::getline(iss, p, TOON_PATH_SEPARATOR)) {
            paths.push_back(p);
        }
    }

    return paths;
}

} // namespace Toon