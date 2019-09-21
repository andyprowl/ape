#include "SampleAssetBuilder.hpp"

#include "AssetLoader.hpp"
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

class StatefulAssetBuilder
{

public:

    auto build()
        -> AssetRepository;

private:

    auto preventReallocation()
        -> void;

    auto createGroundTileModels()
        -> void;

    auto createConcreteGroundTileModel(Shape const & shape)
        -> Model &;

    auto createWoodenFloorTileModel(Shape const & shape)
        -> Model &;

    auto createConcreteGroundMaterial()
        -> Material &;

    auto createWoodenFloorMaterial()
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

    auto createTextureFromLocalFile(std::string filename)
        -> Texture &;

private:

    AssetRepository assets;

};

auto StatefulAssetBuilder::build()
    -> AssetRepository
{
    preventReallocation();

    createGroundTileModels();

    createContainerModel();

    createLampModel();

    createFlashlightModel();

    return std::move(assets);
}

auto StatefulAssetBuilder::preventReallocation()
    -> void
{
    assets.shapes.reserve(10u);

    assets.textures.reserve(10u);

    assets.materials.reserve(10u);

    assets.meshes.reserve(10u);

    assets.models.reserve(10u);
}

auto StatefulAssetBuilder::createGroundTileModels()
    -> void
{
    auto box = makeBox(NormalDirection::outbound, {5.0f, 0.01f, 5.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    createConcreteGroundTileModel(shape);

    createWoodenFloorTileModel(shape);
}

auto StatefulAssetBuilder::createConcreteGroundTileModel(Shape const & shape)
    -> Model &
{
    auto const & material = createConcreteGroundMaterial();

    auto const & mesh = assets.meshes.emplace_back("Concrete Ground Tile", shape, material);

    return createTrivialModel("Concrete Ground Tile", mesh);
}

auto StatefulAssetBuilder::createWoodenFloorTileModel(Shape const & shape)
    -> Model &
{
    auto const & material = createWoodenFloorMaterial();

    auto const & mesh = assets.meshes.emplace_back("Wooden Floor Tile", shape, material);

    return createTrivialModel("Wooden Floor Tile", mesh);
}

auto StatefulAssetBuilder::createConcreteGroundMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = createTextureFromLocalFile("ConcreteGround.jpg");

    auto const & diffuseMap = &texture;

    auto const & specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createWoodenFloorMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = createTextureFromLocalFile("WoodenFloor.jpg");

    auto const & diffuseMap = &texture;

    auto const & specularMap = &texture;

    auto const shininess = 16.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createContainerModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::outbound, {1.0f, 1.0f, 1.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createContainerMaterial();

    auto const & mesh = assets.meshes.emplace_back("Container", shape, material);

    return createTrivialModel("Container", mesh);
}

auto StatefulAssetBuilder::createContainerMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const & diffuseMap = createTextureFromLocalFile("Container.Diffuse.png");

    auto const & specularMap = createTextureFromLocalFile("Container.Specular.png");

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, &diffuseMap, &specularMap, shininess);
}

auto StatefulAssetBuilder::createLampModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::inbound, {0.2f, 0.2f, 0.2f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createLampMaterial();

    auto const & mesh = assets.meshes.emplace_back("Lamp", shape, material);

    return createTrivialModel("Lamp", mesh);
}

auto StatefulAssetBuilder::createLampMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createTextureFromLocalFile("Lamp.png");

    auto diffuseMap = &texture;

    auto specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createFlashlightModel()
    -> Model &
{
    auto box = makeBox(NormalDirection::inbound, {0.3f, 0.1f, 0.1f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createFlashlightMaterial();

    auto const & mesh = assets.meshes.emplace_back("Flashlight", shape, material);

    return createTrivialModel("Flashlight", mesh);
}

auto StatefulAssetBuilder::createFlashlightMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createTextureFromLocalFile("Flashlight.png");

    auto const diffuseMap = &texture;

    auto const specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createTrivialModel(std::string name, Mesh const & mesh)
    -> Model &
{
    auto rootPart = ModelPart{"", {&mesh}, {}, glm::mat4{1.0f}};

    auto model = Model{std::move(name), "", std::move(rootPart)};

    return assets.models.emplace_back(std::move(model));
}

auto StatefulAssetBuilder::createTextureFromLocalFile(std::string filename)
    -> Texture &
{
    auto filepath = std::string{textureFolder} + "/" + filename;

    return assets.textures.emplace_back(std::move(filepath));
}

} // unnamed namespace

auto SampleAssetBuilder::build() const
    -> SampleAssetCollection
{
    auto collection = SampleAssetCollection{};

    auto builder = StatefulAssetBuilder{};

    collection.generalAssets = builder.build();

    auto const loader = AssetLoader{};

    collection.nanosuitAssets = loader.load(std::string{modelFolder} + "/nanosuit.obj");

    return collection;
}
