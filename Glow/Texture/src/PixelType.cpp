#include <Glow/Texture/PixelType.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace glow
{

auto const pixelTypeMap = std::unordered_map<PixelType, GLenum>{
    {PixelType::unsignedByte, GL_UNSIGNED_BYTE},
    {PixelType::floatingPoint, GL_FLOAT}};

auto convertToOpenGLPixelType(PixelType const type)
    -> GLenum
{
    auto const it = pixelTypeMap.find(type);

    if (it == std::cend(pixelTypeMap))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

auto convertFromOpenGLPixelType(GLenum const type)
    -> PixelType
{
    for (auto const & entry : pixelTypeMap)
    {
        if (entry.second == type)
        {
            return entry.first;
        }
    }

    assert(false);

    return PixelType::unknown;
}

} // namespace glow
