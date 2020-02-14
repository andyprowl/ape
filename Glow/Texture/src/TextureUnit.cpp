#include <Glow/Texture/TextureUnit.hpp>

#include <Glow/Texture/CubeTexture.hpp>
#include <Glow/Texture/Texture.hpp>

#include <glad/glad.h>

namespace glow
{

TextureUnit::TextureUnit(int const unitIndex)
    : unitIndex{unitIndex}
    , activeTexture2d{0}
    , activeCubeTexture{0}
{
}

auto TextureUnit::getIndex() const
    -> int
{
    return unitIndex;
}

auto TextureUnit::activate()
    -> void
{
    glActiveTexture(GL_TEXTURE0 + unitIndex);
}

auto TextureUnit::setTexture(Texture const * texture)
    -> void
{
    setTexture(texture, activeTexture2d);
}

auto TextureUnit::setTexture(CubeTexture const * texture)
    -> void
{
    setTexture(texture, activeCubeTexture);
}

template<typename TextureType>
auto TextureUnit::setTexture(TextureType const * texture, TextureType const * & activeTexture)
    -> void
{
    if (texture == activeTexture)
    {
        return;
    }

    if (texture == nullptr)
    {
        activate();

        this->resetActiveTexture(activeTexture);

        return;
    }

    activate();

    texture->bind();

    activeTexture = texture;
}

template<typename TextureType>
auto TextureUnit::resetActiveTexture(TextureType const * & activeTexture)
    -> void
{
    if (activeTexture != nullptr)
    {
        activeTexture->unbind();
    }

    activeTexture = nullptr;
}

} // namespace glow
