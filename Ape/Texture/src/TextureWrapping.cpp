#include <Ape/Texture/TextureWrapping.hpp>

#include <glad/glad.h>

#include <cassert>
#include <unordered_map>

namespace ape
{

auto const textureWrapping = std::unordered_map<TextureWrapping, GLint>{
    {TextureWrapping::repeat, GL_REPEAT},
    {TextureWrapping::clampToEdge, GL_CLAMP_TO_EDGE}};

auto convertToOpenGLTextureWrapping(TextureWrapping const wrapping)
    -> GLint
{
    auto const it = textureWrapping.find(wrapping);

    if (it == std::cend(textureWrapping))
    {
        assert(false);

        return 0;
    }

    return it->second;
}

} // namespace ape
