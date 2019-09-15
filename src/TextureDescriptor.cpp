#include "TextureDescriptor.h"

#include <cassert>

auto readTextureDescriptor(std::string const & filename)
    -> TextureDescriptor
{
    const auto filepath = std::string{textureFolder} + "/" + filename;

    stbi_set_flip_vertically_on_load(true);

    auto size = Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const bytes = stbi_load(filepath.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (bytes == nullptr)
    {
        throw CouldNotLoadTexture{filename};
    }

    auto format = determineFormat(numOfChannels);

    return {size, numOfChannels, format, TextureBytesPtr{bytes}};
}

auto determineFormat(int const numOfChannels)
    -> GLenum
{
    switch (numOfChannels)
    {
        case 1:
        {
            return GL_RED;
        }

        case 3:
        {
            return GL_RGB;
        }

        case 4:
        {
            return GL_RGBA;
        }

        default:
        {
            assert(false);

            return GL_RED;
        }
    }
}
