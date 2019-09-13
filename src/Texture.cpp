#include "Texture.hpp"

#include "glad/glad.h"
#include "stb/stbimage.h"

auto readTextureData(std::string const & filename)
    -> TextureData
{
    const auto filepath = std::string{textureFolder} + "/" + filename;

    auto tex = TextureData{};
    
    stbi_set_flip_vertically_on_load(true);

    auto const data = stbi_load(filepath.c_str(), &tex.width, &tex.height, &tex.numOfChannels, 0);

    tex.data.reset(data);

    if (tex.data == nullptr)
    {
        throw CouldNotLoadTexture{filename};
    }

    return tex;
}

auto makeTexture(std::string const & filename, int const format)
    -> int
{
    auto texData = readTextureData(filename);

    auto textureId = unsigned int{};

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        texData.width,
        texData.height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        texData.data.get());

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}