#include <Rave/RaveCore/RaveAssetCollection.hpp>

#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Shape/BoxShapeBuilder.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/Texture/TextureReader.hpp>

#include <Basix/Memory/Null.hpp>

namespace rave
{

namespace
{

auto const simpleAssetRepositoryName = "Simple";
auto const nanosuitModelName = "Nanosuit";
auto const dragonModelName = "Dragon";
auto const spaceshipModelName = "Spaceship";
auto const dynoModelName = "Dyno";
auto const castleModelName = "Castle";
auto const tavernModelName = "Tavern";
auto const houseModelName = "House";
auto const cottageModelName = "Cottage";
auto const bridgeModelName = "Bridge";
auto const libertyStatueModelName = "LibertyStatus";
auto const sponzaModelName = "Sponza";

auto makeBox(ape::NormalDirection const normalDirection, glm::vec3 const & size)
    -> ape::Shape
{
    auto const builder = ape::BoxShapeBuilder{};

    return builder.build(normalDirection, size);
}

auto resolveModelFilepath(std::filesystem::path relativePath)
    -> std::filesystem::path
{
    return std::filesystem::path{resourceFolder} / "models" / std::move(relativePath);
}

class StatefulSimpleAssetBuilder
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

    auto createColorTextureFromLocalFile(std::string filename)
        -> glow::Texture &;

    auto createDataTextureFromLocalFile(std::string filename)
        -> glow::Texture &;

    auto createTextureFromLocalFile(std::string filename, glow::ColorSpace colorSpace)
        -> glow::Texture &;

private:

    ape::AssetRepository assets;

    glow::TextureReader textureReader;

};

auto StatefulSimpleAssetBuilder::build()
    -> ape::AssetRepository
{
    preventReallocation();

    createGroundTileModels();

    createContainerModel();

    createLampModel();

    createFlashlightModel();

    return std::move(assets);
}

auto StatefulSimpleAssetBuilder::preventReallocation()
    -> void
{
    assets.shapes.reserve(10u);

    assets.textures.reserve(10u);

    assets.materials.reserve(10u);

    assets.meshes.reserve(10u);

    assets.models.reserve(10u);
}

auto StatefulSimpleAssetBuilder::createGroundTileModels()
    -> void
{
    auto box = makeBox(ape::NormalDirection::outbound, {5.0f, 0.1f, 5.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    createConcreteGroundTileModel(shape);

    createWoodenFloorTileModel(shape);
}

auto StatefulSimpleAssetBuilder::createConcreteGroundTileModel(ape::Shape const & shape)
    -> ape::Model &
{
    auto const & material = createConcreteGroundMaterial();

    return createSingleMeshModel(shape, material, "Concrete Floor Tile");
}

auto StatefulSimpleAssetBuilder::createWoodenFloorTileModel(ape::Shape const & shape)
    -> ape::Model &
{
    auto const & material = createWoodenFloorMaterial();

    return createSingleMeshModel(shape, material, "Wooden Floor Tile");
}

auto StatefulSimpleAssetBuilder::createConcreteGroundMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & colorTexture = createColorTextureFromLocalFile("PavementHerringbone.Diffuse.png");

    auto const & diffuseMap = colorTexture;

    auto const & specularMap = colorTexture;

    auto const & normalMap = createDataTextureFromLocalFile("PavementHerringbone.Normal.png");

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(
        ambientColor,
        &diffuseMap,
        &specularMap,
        &normalMap,
        shininess);
}

auto StatefulSimpleAssetBuilder::createWoodenFloorMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & colorTexture = createColorTextureFromLocalFile("WoodenFloor.Diffuse.png");

    auto const & diffuseMap = colorTexture;

    auto const & specularMap = colorTexture;

    auto const & normalMap = createDataTextureFromLocalFile("WoodenFloor.Normal.png");
    
    auto const shininess = 16.0f;

    return assets.materials.emplace_back(
        ambientColor,
        &diffuseMap,
        &specularMap,
        &normalMap,
        shininess);
}

auto StatefulSimpleAssetBuilder::createContainerModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::outbound, {1.0f, 1.0f, 1.0f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createContainerMaterial();

    return createSingleMeshModel(shape, material, "Container");
}

auto StatefulSimpleAssetBuilder::createContainerMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const & diffuseMap = createColorTextureFromLocalFile("Container.Diffuse.png");

    auto const & specularMap = createColorTextureFromLocalFile("Container.Specular.png");

    auto const & normalMap = createDataTextureFromLocalFile("Container.Normal.png");

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(
        ambientColor,
        &diffuseMap,
        &specularMap,
        &normalMap,
        shininess);
}

auto StatefulSimpleAssetBuilder::createLampModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::inbound, {0.2f, 0.2f, 0.2f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createLampMaterial();

    return createSingleMeshModel(shape, material, "Lamp");
}

auto StatefulSimpleAssetBuilder::createLampMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createColorTextureFromLocalFile("Lamp.png");

    auto diffuseMap = &texture;

    auto specularMap = &texture;

    auto const normalMap = basix::nullPtr<glow::Texture>;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(
        ambientColor,
        diffuseMap,
        specularMap,
        normalMap,
        shininess);
}

auto StatefulSimpleAssetBuilder::createFlashlightModel()
    -> ape::Model &
{
    auto box = makeBox(ape::NormalDirection::inbound, {0.3f, 0.1f, 0.1f});

    auto const & shape = assets.shapes.emplace_back(std::move(box));

    auto const & material = createFlashlightMaterial();

    return createSingleMeshModel(shape, material, "Flashlight");
}

auto StatefulSimpleAssetBuilder::createFlashlightMaterial()
    -> ape::Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = createColorTextureFromLocalFile("Flashlight.png");

    auto const diffuseMap = &texture;

    auto const specularMap = &texture;

    auto const normalMap = basix::nullPtr<glow::Texture>;

    auto const shininess = 32.0f;

    return assets.materials.emplace_back(
        ambientColor,
        diffuseMap,
        specularMap,
        normalMap,
        shininess);
}

auto StatefulSimpleAssetBuilder::createSingleMeshModel(
    ape::Shape const & shape,
    ape::Material const & material,
    std::string name)
    -> ape::Model &
{
    auto const & mesh = assets.meshes.emplace_back(std::move(name), shape, material);

    return createTrivialModelFromMesh(mesh);
}

auto StatefulSimpleAssetBuilder::createTrivialModelFromMesh(ape::Mesh const & mesh)
    -> ape::Model &
{
    auto const meshName = mesh.getName();

    auto rootPart = ape::ModelPart{meshName, {&mesh}, {}, glm::mat4{1.0f}};

    auto model = ape::Model{std::move(rootPart), meshName};

    return assets.models.emplace_back(std::move(model));
}

auto StatefulSimpleAssetBuilder::createColorTextureFromLocalFile(std::string filename)
    -> glow::Texture &
{
    return createTextureFromLocalFile(std::move(filename), glow::ColorSpace::perceptual);
}

auto StatefulSimpleAssetBuilder::createDataTextureFromLocalFile(std::string filename)
    -> glow::Texture &
{
    return createTextureFromLocalFile(std::move(filename), glow::ColorSpace::linear);
}

auto StatefulSimpleAssetBuilder::createTextureFromLocalFile(
    std::string filename,
    glow::ColorSpace const colorSpace)
    -> glow::Texture &
{
    auto const filepath = std::filesystem::path{resourceFolder} / "textures" / filename;

    // Let the number of mipmap levels be deduced from the image size.
    auto const numOfMipmapLevels = 0;

    auto texture = textureReader.read(
        filepath,
        colorSpace,
        glow::TextureFiltering{
            glow::TextureMagnificationFilter::linear,
            glow::TextureMinificationFilter::linearMipmapLinear},
        glow::TextureWrapping::repeat,
        numOfMipmapLevels,
        filepath.string());

    return assets.textures.emplace_back(std::move(texture));
}

} // unnamed namespace

RaveAssetCollection::RaveAssetCollection(bool excludeSponza)
{
    createSimpleAssets();

    loadAssets("Nanosuit/nanosuit.obj", nanosuitModelName);
    
    loadAssets("Dragon/Dragon.obj", dragonModelName);

    loadAssets("Spaceship/Spaceship.obj", spaceshipModelName);

    loadAssets("Dyno/Apatosaurus.obj", dynoModelName);

    loadAssets("Castle/Castle.obj", castleModelName);

    loadAssets("Tavern/Tavern.obj", tavernModelName);

    loadAssets("House/House.obj", houseModelName);

    loadAssets("Cottage/Cottage.obj", cottageModelName);

    loadAssets("Bridge/Bridge.obj", bridgeModelName);

    loadAssets("LibertyStatue/LibertyStatue.obj", libertyStatueModelName);

    if (not excludeSponza)
    {
        loadAssets("Sponza/sponza.obj", sponzaModelName);
    }
}

auto RaveAssetCollection::getSimpleAssets()
    -> ape::AssetRepository &
{
    return getAssets(simpleAssetRepositoryName);
}

auto RaveAssetCollection::getNanosuitModel()
    -> ape::Model &
{
    return getAssets(nanosuitModelName).models[0];
}

auto RaveAssetCollection::getDragonModel()
    -> ape::Model &
{
    return getAssets(dragonModelName).models[0];
}

auto RaveAssetCollection::getSpaceshipModel()
    -> ape::Model &
{
    return getAssets(spaceshipModelName).models[0];
}

auto RaveAssetCollection::getDynoModel()
    -> ape::Model &
{
    return getAssets(dynoModelName).models[0];
}

auto RaveAssetCollection::getCastleModel()
    -> ape::Model &
{
    return getAssets(castleModelName).models[0];
}

auto RaveAssetCollection::getTavernModel()
    -> ape::Model &
{
    return getAssets(tavernModelName).models[0];
}

auto RaveAssetCollection::getHouseModel()
    -> ape::Model &
{
    return getAssets(houseModelName).models[0];
}

auto RaveAssetCollection::getCottageModel()
    -> ape::Model &
{
    return getAssets(cottageModelName).models[0];
}

auto RaveAssetCollection::getBridgeModel()
    -> ape::Model &
{
    return getAssets(bridgeModelName).models[0];
}

auto RaveAssetCollection::getLibertyStatueModel()
    -> ape::Model &
{
    return getAssets(libertyStatueModelName).models[0];
}

auto RaveAssetCollection::getSponzaModel()
    -> ape::Model &
{
    return getAssets(sponzaModelName).models[0];
}

void RaveAssetCollection::createSimpleAssets()
{
    auto builder = StatefulSimpleAssetBuilder{};

    auto repository = builder.build();

    addAssets(simpleAssetRepositoryName, std::move(repository));
}

void RaveAssetCollection::loadAssets(std::filesystem::path const & path, std::string modelName)
{
    auto const resolvedPath = resolveModelFilepath(path);

    auto repository = loader.load(resolvedPath, modelName);

    addAssets(std::move(modelName), std::move(repository));
}

} // namespace rave
