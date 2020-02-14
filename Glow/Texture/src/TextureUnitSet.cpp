#include <Glow/Texture/TextureUnitSet.hpp>

#include <glad/glad.h>

namespace glow
{

namespace
{

auto makeTextureUnits()
    -> std::vector<TextureUnit>
{
    auto units = std::vector<TextureUnit>{};

    units.reserve(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

    for (auto i = 0; i < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS; ++i)
    {
        units.push_back(TextureUnit{i});
    }

    return units;
}

} // unnamed namespace

TextureUnitSet::TextureUnitSet()
    : units{makeTextureUnits()}
{
}

auto TextureUnitSet::getUnit(int const index)
    -> TextureUnit &
{
    return units[index];
}

auto TextureUnitSet::operator [] (int const index)
    -> TextureUnit &
{
    return units[index];
}

} // namespace glow
