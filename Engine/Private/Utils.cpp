#include <Temporality/Utils.hpp>
#include <Temporality/Log.hpp>

#include <sstream>

namespace Temporality {

TEMPORALITY_ENGINE_API
std::vector<std::string> GetAssetPaths()
{
    static std::vector<std::string> paths;
    if (!paths.empty()) {
        return paths;
    }

    paths.push_back(std::string());

    const char * path = getenv("TEMPORALITY_ASSET_PATH");
    LogVerbose("TEMPORALITY_ASSET_PATH=%s", path);

    if (path) {
        std::istringstream iss(path);
        std::string p;
        while (std::getline(iss, p, TEMPORALITY_PATH_SEPARATOR)) {
            paths.push_back(p);
        }
    }

    return paths;
}

} // namespace Temporality