#include <Ape/Rendering/Lighting/BlinnPhongShaderCollectionPopulator.hpp>

#include <Ape/Rendering/Lighting/BlinnPhongShaderCollection.hpp>
#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <Basix/Text/Substring.hpp>

#include <iostream>

namespace ape
{

BlinnPhongShaderCollectionPopulator::BlinnPhongShaderCollectionPopulator(
    BlinnPhongShaderCollection & collection)
    : collection{&collection}
{
    addAllShaders();
}

auto BlinnPhongShaderCollectionPopulator::addAllShadersInFolder(
    std::filesystem::path const & folderPath) const
    -> void
{
    for (auto const & entry : std::filesystem::recursive_directory_iterator{folderPath})
    {
        if (not entry.is_directory())
        {
            continue;
        }

        addShaderInFolder(entry);
    }
}

auto BlinnPhongShaderCollectionPopulator::addAllShaders() const
    -> void
{
    return addAllShadersInFolder(std::filesystem::path{resourceFolder} / "Shaders/BlinnPhong");
}

auto BlinnPhongShaderCollectionPopulator::addShaderInFolder(
    std::filesystem::path const & folderPath) const
    -> void
{
    auto vertexShaderPath = glow::VertexShaderPath{};
    
    auto fragmentShaderPaths = std::vector<glow::FragmentShaderPath>{};

    for (auto const & entry : std::filesystem::recursive_directory_iterator{folderPath})
    {
        if (not entry.is_regular_file())
        {
            continue;
        }

        auto path = entry.path();

        if (path.string().find("Vertex") != std::string::npos)
        {
            vertexShaderPath = glow::VertexShaderPath{std::move(path)};
        }
        else if (path.string().find("Fragment") != std::string::npos)
        {
            fragmentShaderPaths.emplace_back(std::move(path));
        }
    }

    auto shader = std::make_unique<BlinnPhongShaderProgram>(
        std::move(vertexShaderPath),
        std::move(fragmentShaderPaths));

    collection->addShader(std::move(shader));
}

} // namespace ape
