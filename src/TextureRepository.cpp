#include "TextureRepository.hpp"

#include <algorithm>

auto TextureRepository::addTexture(Texture texture)
    -> Texture &
{
    auto filename = texture.getFilename();

    if (findTexture(filename) != nullptr)
    {
        throw TextureFilenameNotUnique{std::move(filename)};
    }

    textures.push_back(std::move(texture));

    return textures.back();
}

auto TextureRepository::findTexture(std::string const & filename) const
    -> Texture const *
{
    auto it = std::find_if(
        std::cbegin(textures),
        std::cend(textures),
        [&filename] (const Texture & t)
    {
        return (t.getFilename() == filename);
    });

    if (it == std::cend(textures))
    {
        return nullptr;
    }

    return &(*it);
}
