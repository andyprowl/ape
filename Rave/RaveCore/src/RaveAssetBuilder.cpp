#include <Rave/RaveCore/RaveAssetBuilder.hpp>

#include <Ape/World/AssetLoader//AssetLoader.hpp>
#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Shape/BoxBuilder.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/Texture/TextureReader.hpp>

namespace rave
{

namespace
{

auto makeBox(ape::NormalDirection const normalDirection, glm::vec3 const & size)
    -> ape::Shape
{
    auto const builder = ape::BoxBuilder{};

    return builder.build(normalDirection, size);
}

class StatefulAssetBuilder
{

public:

    auto build()
        -> ape::AssetRepository;

private:

    auto preventReallocation()
        -> void;

    auto createGroundTileModels()
        -> void;

    auto createConcreteGroundTileModel(ape::Shape const & shape)
        -> ape::Model &;

    auto createWoodenFloorTileModel(ape::Shape const & shape)
        -> ape::Model &;

    auto createConcreteGroundMaterial()
        -> ape::Material &;

    auto createWoodenFloorMaterial()
        -> ape::Material &;

    auto createContainerModel()
        -> ape::Model &;

    auto createContainerMaterial()
        -> ape::Material &;

    auto createLampModel()
        -> ape::Model &;

    auto createLampMaterial()
        -> ape::Material &;

    auto createFlashlightModel()
        -> ape::Model &;

    auto createFlashlightMaterial()
        -> ape::Material &;

    auto createSingleMeshModel(
        ape::Shape const & shape,
        ape::Material const & material,
        std::string name)
        -> ape::Model &;

    auto createTrivialModelFromMesh(ape::Mesh const & mesh)
        -> ape::Model &;

    auto createTextureFromLocalFile(std::string filename)
        -> glow::Texture &;

private:

    ape::AssetRepository assets;

    glow::TextureReader textureReader;

};

auto StatefulAssetBuilder::build()
    -> ape::AssetRepository
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
    auto box = makeBox(ape::NormalDirection::outbound, {5.0f, 0.01f, 5.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    createConcreteGroundTileModel(shape);

    createWoodenFloorTileModel(shape);
}

auto StatefulAssetBuilder::createConcreteGroundTileModel(ape::Shape const & shape)
    -> ape::Model &
{
    auto const & material = createConcreteGroundMaterial();

    return createSingleMeshModel(shape, material, "Concrete Floor Tile");
}

auto StatefulAssetBuilder::createWoodenFloorTileModel(ape::Shape const & shape)
    -> ape::Model &
{
    auto const & material = createWoodenFloorMaterial();

    return createSingleMeshModel(shape, material, "Wooden Floor Tile");
}

auto StatefulAssetBuilder::createConcreteGroundMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = createTextureFromLocalFile("ConcreteGround.jpg");

    auto const & diffuseMap = &texture;

    auto const & specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createWoodenFloorMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = createTextureFromLocalFile("WoodenFloor.jpg");

    auto const & diffuseMap = &texture;

    auto const & specularMap = &texture;

    auto const shininess = 16.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createContainerModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::outbound, {1.0f, 1.0f, 1.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createContainerMaterial();

    return createSingleMeshModel(shape, material, "Container");
}

auto StatefulAssetBuilder::createContainerMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const & diffuseMap = createTextureFromLocalFile("Container.Diffuse.png");

    auto const & specularMap = createTextureFromLocalFile("Container.Specular.png");

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, &diffuseMap, &specularMap, shininess);
}

auto StatefulAssetBuilder::createLampModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::inbound, {0.2f, 0.2f, 0.2f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createLampMaterial();

    return createSingleMeshModel(shape, material, "Lamp");
}

auto StatefulAssetBuilder::createLampMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createTextureFromLocalFile("Lamp.png");

    auto diffuseMap = &texture;

    auto specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createFlashlightModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::inbound, {0.3f, 0.1f, 0.1f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createFlashlightMaterial();

    return createSingleMeshModel(shape, material, "Flashlight");
}

auto StatefulAssetBuilder::createFlashlightMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createTextureFromLocalFile("Flashlight.png");

    auto const diffuseMap = &texture;

    auto const specularMap = &texture;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(ambientColor, diffuseMap, specularMap, shininess);
}

auto StatefulAssetBuilder::createSingleMeshModel(
    ape::Shape const & shape,
    ape::Material const & material,
    std::string name)
    -> ape::Model &
{
    auto const & mesh = assets.meshes.emplace_back(std::move(name), shape, material);

    return createTrivialModelFromMesh(mesh);
}

auto StatefulAssetBuilder::createTrivialModelFromMesh(ape::Mesh const & mesh)
    -> ape::Model &
{
    auto const meshName = mesh.getName();

    auto rootPart = ape::ModelPart{meshName, {&mesh}, {}, glm::mat4{1.0f}};

    auto model = ape::Model{std::move(rootPart), meshName};

    return assets.models.emplace_back(std::move(model));
}

auto StatefulAssetBuilder::createTextureFromLocalFile(std::string filename)
    -> glow::Texture &
{
    auto const filepath = std::filesystem::path{resourceFolder} / "textures" / filename;

    auto const storageType = glow::TextureStorageType::immutable;

    auto const colorSpace = glow::ColorSpace::perceptual;

    auto texture = textureReader.read(filepath, storageType, colorSpace);

    return assets.textures.emplace_back(std::move(texture));
}

auto resolveModelFilepath(std::filesystem::path relativePath)
    -> std::filesystem::path
{
    return std::filesystem::path{resourceFolder} / "models" / std::move(relativePath);
}

auto collectShapes(ape::AssetRepository & repository, RaveAssetCollection & collection)
    -> void
{
    collection.shapes.reserve(collection.shapes.size() + repository.shapes.size());

    for (auto & shape : repository.shapes)
    {
        collection.shapes.push_back(&shape);
    }
}

auto createSimpleAssets(RaveAssetCollection & collection)
    -> void
{
    auto builder = StatefulAssetBuilder{};

    collection.generalAssets = builder.build();

    collectShapes(collection.generalAssets, collection);
}

auto loadAssets(
    std::filesystem::path const & path,
    std::string modelName,
    ape::AssetRepository & destination,
    RaveAssetCollection & collection)
    -> void
{
    auto const loader = ape::AssetLoader{};

    auto const nanosuiteFilepath = resolveModelFilepath(path);

    destination = loader.load(nanosuiteFilepath, modelName);

    collectShapes(destination, collection);
}

} // unnamed namespace

RaveAssetBuilder::RaveAssetBuilder()
    : RaveAssetBuilder{true}
{
}

RaveAssetBuilder::RaveAssetBuilder(bool const doNotIncludeSponza)
    : doNotIncludeSponza{doNotIncludeSponza}
{
}

auto RaveAssetBuilder::build() const
    -> RaveAssetCollection
{
    auto collection = RaveAssetCollection{};

    createSimpleAssets(collection);

    loadAssets("Nanosuit/nanosuit.obj", "Nanosuit", collection.nanosuitAssets, collection);

    loadAssets("Dragon/Dragon.obj", "Dragon", collection.dragonAssets, collection);

    loadAssets("Spaceship/Spaceship.obj", "Spaceship", collection.spaceshipAssets, collection);

    loadAssets("Dyno/Apatosaurus.obj", "Dyno", collection.dynoAssets, collection);

    loadAssets("Castle/Castle.obj", "Castle", collection.castleAssets, collection);

    if (!doNotIncludeSponza)
    {
        loadAssets("Sponza/sponza.obj", "Sponza", collection.sponzaAssets, collection);
    }

    return collection;
}

} // namespace rave
