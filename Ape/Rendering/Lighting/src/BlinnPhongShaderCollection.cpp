#include <Ape/Rendering/Lighting/BlinnPhongShaderCollection.hpp>

#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Basix/Range/Search.hpp>
#include <Basix/Range/Transform.hpp>

namespace ape
{

auto BlinnPhongShaderCollection::addShader(std::unique_ptr<BlinnPhongShaderProgram> shader)
    -> void
{
    shaders.push_back(std::move(shader));

    onShaderAdded.fire(*shaders.back());
}

auto BlinnPhongShaderCollection::getNumOfShaders() const
    -> int
{
    return static_cast<int>(shaders.size());
}

auto BlinnPhongShaderCollection::getShader(int const i)
    -> BlinnPhongShaderProgram &
{
    return *(shaders[i]);
}

auto BlinnPhongShaderCollection::getShaderIndex(BlinnPhongShaderProgram const & shader) const
    -> int
{
    auto const it = basix::findIf(shaders, [&shader] (auto const & p)
    {
        return (p.get() == &shader);
    });

    if (it == std::cend(shaders))
    {
        throw ShaderNotContained{};
    }

    auto const index = std::distance(std::cbegin(shaders), it);

    return static_cast<int>(index);
}

} // namespace ape
