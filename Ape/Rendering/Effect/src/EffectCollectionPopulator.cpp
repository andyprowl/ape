#include <Ape/Rendering/Effect/EffectCollectionPopulator.hpp>

#include <Ape/Rendering/Effect/EffectCollection.hpp>

#include <Basix/Text/Substring.hpp>

namespace ape
{

namespace
{

auto const effectShaderSuffix = ".Fragment.glsl";

auto isEffectShaderFile(std::filesystem::directory_entry const & entry)
    -> bool
{
    if (not entry.is_regular_file())
    {
        return false;
    }

    return basix::endsWith(entry.path().string(), effectShaderSuffix);
}

} // unnamed namespace

EffectCollectionPopulator::EffectCollectionPopulator(EffectCollection & collection)
    : collection{&collection}
{
}

auto EffectCollectionPopulator::addAllEffectsInFolder(
    std::filesystem::path const & folderPath) const
    -> void
{
    for (auto const & entry : std::filesystem::recursive_directory_iterator{folderPath})
    {
        if (isEffectShaderFile(entry))
        {
            auto shaderPath = entry.path();

            auto effect = EffectShaderProgram{std::move(shaderPath)};

            collection->addEffect(std::move(effect));
        }
    }
}

auto EffectCollectionPopulator::addAllStandardEffects() const
    -> void
{
    return addAllEffectsInFolder(std::filesystem::path{resourceFolder} / "Shaders/Effects");
}

} // namespace ape
