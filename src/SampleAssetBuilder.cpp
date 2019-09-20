#include "SampleAssetBuilder.hpp"

#include "BoxBuilder.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Normal.hpp"
#include "Shape.hpp"

namespace
{

auto makeBox(NormalDirection const normalDirection, glm::vec3 const & size)
    -> Shape
{
    auto const builder = BoxBuilder{};

    return builder.build(normalDirection, size);
}

class StatefulSampleAssetBuilder
{

public:

    auto build()
        -> AssetRepository;

private:

    auto preventReallocation()
        -> void;

    auto createGroundTileModel()
        -> Model &;

    auto createGroundMaterial()
        -> Material &;

    auto createContainerModel()
        -> Model &;

    auto createContainerMaterial()
        -> Material &;

    auto createLampModel()
        -> Model &;

    auto createLampMaterial()
        -> Material &;

    auto createFlashlightModel()
        -> Model &;

    auto createFlashlightMaterial()
        -> Material &;

    auto createTrivialModel(std::string name, Mesh const & mesh)
        -> Model &;

private:

    AssetRepository assets;

};

auto StatefulSampleAssetBuilder::build()
    -> AssetRepository
{
    preventReallocation();

    createGroundTileModel();

    createContainerModel();

    createLampModel();

    createFlashlightModel();

    return std::move(assets);
}

auto StatefulSampleAssetBuilder::preventReallocation()
    -> void
{
    assets.shapes.reserve(4u);

    assets.textures.reserve(5u);

    assets.materials.reserve(4u);

    assets.meshes.reserve(4u);

    assets.models.reserve(4u);
}

auto StatefulSampleAssetBuilder::createGroundTileModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::outbound, {5.0f, 0.01f, 5.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createGroundMaterial();

    auto const & mesh = assets.meshes.emplace_back("Ground Tile", shape, material);

    return createTrivialModel("Ground Tile", mesh);
}

auto StatefulSampleAssetBuilder::createGroundMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = assets.textures.emplace_back("ConcreteGround.jpg");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 16.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulSampleAssetBuilder::createContainerModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::outbound, {1.0f, 1.0f, 1.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createContainerMaterial();

    auto const & mesh = assets.meshes.emplace_back("Container", shape, material);

    return createTrivialModel("Container", mesh);
}

auto StatefulSampleAssetBuilder::createContainerMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const & diffuseMap = assets.textures.emplace_back("Container.Diffuse.png");

    auto const & specularMap = assets.textures.emplace_back("Container.Specular.png");

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulSampleAssetBuilder::createLampModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::inbound, {0.2f, 0.2f, 0.2f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createLampMaterial();

    auto const & mesh = assets.meshes.emplace_back("Lamp", shape, material);

    return createTrivialModel("Lamp", mesh);
}

auto StatefulSampleAssetBuilder::createLampMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = assets.textures.emplace_back("Lamp.png");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulSampleAssetBuilder::createFlashlightModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::inbound, {0.3f, 0.1f, 0.1f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createFlashlightMaterial();

    auto const & mesh = assets.meshes.emplace_back("Flashlight", shape, material);

    return createTrivialModel("Flashlight", mesh);
}

auto StatefulSampleAssetBuilder::createFlashlightMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = assets.textures.emplace_back("Flashlight.png");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulSampleAssetBuilder::createTrivialModel(std::string name, Mesh const & mesh)
    -> Model &
{
    auto rootPart = ModelPart{"", {&mesh}, {}, glm::mat4{1.0f}};

    auto model = Model{std::move(name), "", std::move(rootPart)};

    return assets.models.emplace_back(std::move(model));
}

} // unnamed namespace

auto SampleAssetBuilder::build() const
    -> AssetRepository
{
    auto builder = StatefulSampleAssetBuilder{};

    return builder.build();
}
