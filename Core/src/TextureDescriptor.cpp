#include <Core/TextureDescriptor.hpp>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <cassert>

namespace ape
{

auto TextureBytesDeleter::operator () (unsigned char * const data) const
    -> void
{
    stbi_image_free(data);
}

auto readTextureDescriptor(std::string const & filepath)
    -> TextureDescriptor
{
    stbi_set_flip_vertically_on_load(true);

    auto size = Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const bytes = stbi_load(filepath.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (bytes == nullptr)
    {
        throw CouldNotLoadTexture{filepath};
    }

    auto format = determineFormat(numOfChannels);

    return {size, numOfChannels, format, TextureBytesPtr{bytes}};
}

auto determineFormat(int const numOfChannels)
    -> unsigned int
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

} // namespace ape
