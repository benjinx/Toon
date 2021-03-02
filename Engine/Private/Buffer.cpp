#include <Toon/Buffer.hpp>
#include <Toon/Log.hpp>

#include <Toon/String.hpp>

namespace Toon {

TOON_ENGINE_API
bool Buffer::ReadFrom(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::Download) {
        ToonLogError("Unable to read data from buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        ToonLogError("Buffer is not mapped for reading");
        return false;
    }

    memcpy(data, _mappedBufferMemory, length);

    return true;
}

TOON_ENGINE_API
bool Buffer::WriteTo(size_t offset, size_t length, uint8_t * data)
{
    if (_memoryUsage != MemoryUsage::UploadOnce && _memoryUsage != MemoryUsage::UploadOften) {
        ToonLogError("Unable to write data to buffer with MemoryUsage: %s",
            MemoryUsageToString(_memoryUsage));
        return false;
    }

    if (!_mappedBufferMemory) {
        ToonLogError("Buffer is not mapped for writing");
        return false;
    }

    memcpy(_mappedBufferMemory, data, length);

    return true;
}

} // namespace Toon