#include <Ape/World/AssetLoader//TextureCache.hpp>

namespace ape
{

auto TextureCache::registerTexture(glow::Texture & texture, std::filesystem::path const & path)
    -> void
{
    auto const result = cache.emplace(path.string(), &texture);

    if (!result.second)
    {
        throw TexturePathNotUnique{path};
    }
}

auto TextureCache::findTexture(std::filesystem::path const & path) const
    -> glow::Texture *
{
    auto const it = cache.find(path.string());

    if (it == std::cend(cache))
    {
        return nullptr;
    }

    return it->second;
}

} // namespace ape