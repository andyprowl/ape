#include <Ape/Rendering/Lighting/BlinnPhongShaderSelector.hpp>

#include <Ape/Rendering/Lighting/BlinnPhongShaderCollection.hpp>

namespace ape
{

namespace
{

auto getAllShadersFromCollection(BlinnPhongShaderCollection & collection)
    -> std::vector<BlinnPhongShaderProgram *>
{
    auto const numOfShaders = collection.getNumOfShaders();

    auto shaders = std::vector<BlinnPhongShaderProgram *>{};

    shaders.reserve(numOfShaders);

    for (auto i = 0; i < numOfShaders; ++i)
    {
        auto & shader = collection.getShader(i);

        shaders.push_back(&shader);
    }

    return shaders;
}

} // unnamed namespace

BlinnPhongShaderSelector::BlinnPhongShaderSelector(BlinnPhongShaderCollection & collection)
    : collection{&collection}
    , availableShaders{getAllShadersFromCollection(collection)}
    , activeShaderIndex{tryGetFirstShaderIndex()}
{
}

auto BlinnPhongShaderSelector::getCollection() const
    -> BlinnPhongShaderCollection &
{
    return *collection;
}

auto BlinnPhongShaderSelector::getAvailableShaders() const
    -> std::vector<BlinnPhongShaderProgram *> const &
{
    return availableShaders;
}

auto BlinnPhongShaderSelector::getActiveShader() const
    -> BlinnPhongShaderProgram *
{
    if (not activeShaderIndex)
    {
        return nullptr;
    }

    return availableShaders[*activeShaderIndex];
}

auto BlinnPhongShaderSelector::activateShader(int const index)
    -> void
{
    if (index >= static_cast<int>(availableShaders.size()))
    {
        throw BadShaderIndex{index};
    }

    activeShaderIndex = index;

    onActiveShaderChanged.fire(getActiveShader());
}

auto BlinnPhongShaderSelector::activateShader(BlinnPhongShaderProgram const & shader)
    -> void
{
    if (getActiveShader() == &shader)
    {
        return;
    }

    activeShaderIndex = collection->getShaderIndex(shader);

    onActiveShaderChanged.fire(getActiveShader());
}

auto BlinnPhongShaderSelector::activateNextShader()
    -> void
{
    if (not activeShaderIndex)
    {
        activeShaderIndex = tryGetFirstShaderIndex();
    }
    else
    {
        auto const numOfShaders = static_cast<int>(availableShaders.size());

        activeShaderIndex = (*activeShaderIndex + 1) % numOfShaders;
    }

    onActiveShaderChanged.fire(getActiveShader());
}

auto BlinnPhongShaderSelector::activatePreviousShader()
    -> void
{
    if (not activeShaderIndex)
    {
        activeShaderIndex = tryGetLastShaderIndex();
    }
    else
    {
        auto const numOfShaders = static_cast<int>(availableShaders.size());

        activeShaderIndex = (*activeShaderIndex + numOfShaders - 1) % numOfShaders;
    }

    onActiveShaderChanged.fire(getActiveShader());
}

auto BlinnPhongShaderSelector::deactivateShader()
    -> void
{
    activeShaderIndex = std::nullopt;

    onActiveShaderChanged.fire(nullptr);
}

auto BlinnPhongShaderSelector::tryGetFirstShaderIndex() const
    -> std::optional<int>
{
    if (availableShaders.empty())
    {
        return std::nullopt;
    }

    return 0;
}

auto BlinnPhongShaderSelector::tryGetLastShaderIndex() const
    -> std::optional<int>
{
    if (availableShaders.empty())
    {
        return std::nullopt;
    }

    return static_cast<int>(availableShaders.size() - 1);
}

} // namespace ape
