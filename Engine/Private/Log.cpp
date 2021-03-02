#include <Toon/Log.hpp>

namespace Toon {

static std::vector<FILE *> _LogFiles;

TOON_ENGINE_API
bool AddLogFile(const string& filename)
{
    FILE * file = nullptr;
    file = fopen(filename.c_str(), "wt");
    if (file) {
        _LogFiles.push_back(file);
        ToonLogVerbose("Adding log file '%s'", filename);
        return true;
    }

    ToonLogError("Failed to add log file '%s'", filename);
    return false;
}

TOON_ENGINE_API
std::vector<FILE *> GetAllLogFiles()
{
    return _LogFiles;
}

TOON_ENGINE_API
void CloseAllLogFiles()
{
    for (FILE * file : _LogFiles) {
        fclose(file);
    }
    _LogFiles.clear();
}

} // namespace Toon