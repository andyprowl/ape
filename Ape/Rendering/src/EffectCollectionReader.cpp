#include <Ape/Rendering/EffectCollectionReader.hpp>

#include <Foundational/Text/Substring.hpp>

namespace ape
{

namespace
{

const auto effectShaderSuffix = ".Fragment.glsl";

auto isEffectShaderFile(std::filesystem::directory_entry const & entry)
    -> bool
{
    if (!entry.is_regular_file())
    {
        return false;
    }

    return endsWith(entry.path().string(), effectShaderSuffix);
}

} // unnamed namespace

auto EffectCollectionReader::read() const
    -> EffectCollection
{
    auto const effectFolder = resourceFolder "/shaders/effects";

    auto effects = std::vector<EffectShaderProgram>{};

    for (auto const & entry : std::filesystem::recursive_directory_iterator(effectFolder))
    {
        if (isEffectShaderFile(entry))
        {
            auto const filename = entry.path().filename().string();

            effects.emplace_back(filename);
        }
    }

    return EffectCollection{std::move(effects)};
}

} // namespace ape
