#include "TextureRepository.hpp"

auto TextureRepository::addTexture(Texture texture)
    -> Texture &
{
    auto filename = texture.getFilename();

    if (findTexture(filename) != nullptr)
    {
        throw TextureFilenameNotUnique{std::move(filename)};
    }

    auto const insertionResult = textures.emplace(std::move(filename), std::move(texture));

    auto const it = insertionResult.first;

    return it->second;
}

auto TextureRepository::findTexture(std::string const & filename) const
    -> Texture const *
{
    auto it = textures.find(filename);

    if (it == std::cend(textures))
    {
        return nullptr;
    }

    return &it->second;
}
