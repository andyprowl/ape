#include "AssetBuilder.hpp"

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
    
    auto addShapeAsset(Shape shape)
        -> Shape &;

    auto addTextureAsset(std::string const & path)
        -> Texture &;

    auto addMaterialAsset(Material const & material)
        -> Material &;

    auto addMeshAsset(Mesh const & mesh)
        -> Mesh &;

    auto addModelAsset(Model model)
        -> Model &;

private:

    AssetRepository repository;

};

auto StatefulAssetBuilder::build()
    -> AssetRepository
{
    preventReallocation();

    createGroundTileModel();

    createContainerModel();

    createLampModel();

    createFlashlightModel();

    return std::move(repository);
}

auto StatefulAssetBuilder::preventReallocation()
    -> void
{
    repository.getAssets().shapes.reserve(4u);

    repository.getAssets().textures.reserve(5u);

    repository.getAssets().materials.reserve(4u);

    repository.getAssets().meshes.reserve(4u);

    repository.getAssets().models.reserve(4u);
}

auto StatefulAssetBuilder::createGroundTileModel()
    -> Model &
{
    auto const & shape = addShapeAsset(makeBox(NormalDirection::outbound, {5.0f, 0.01f, 5.0f}));

    auto const & material = createGroundMaterial();

    auto const & mesh = addMeshAsset({"Ground Tile", shape, material});

    return createTrivialModel("Ground Tile", mesh);
}

auto StatefulAssetBuilder::createGroundMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};
    
    auto const & texture = addTextureAsset("ConcreteGround.jpg");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 16.0f;

    return addMaterialAsset({ambientColor, diffuseMap, specularMap, shininess});
}

auto StatefulAssetBuilder::createContainerModel()
    -> Model &
{
    auto const & shape = addShapeAsset(makeBox(NormalDirection::outbound, {1.0f, 1.0f, 1.0f}));

    auto const & material = createContainerMaterial();

    auto const & mesh = addMeshAsset({"Container", shape, material});

    return createTrivialModel("Container", mesh);
}

auto StatefulAssetBuilder::createContainerMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 0.5f, 0.31f};

    auto const & diffuseMap = addTextureAsset("Container.Diffuse.png");

    auto const & specularMap = addTextureAsset("Container.Specular.png");

    auto const shininess = 32.0f;

    return addMaterialAsset({ambientColor, diffuseMap, specularMap, shininess});
}

auto StatefulAssetBuilder::createLampModel()
    -> Model &
{
    auto const & shape = addShapeAsset(makeBox(NormalDirection::inbound, {0.2f, 0.2f, 0.2f}));

    auto const & material = createLampMaterial();

    auto const & mesh = addMeshAsset({"Lamp", shape, material});

    return createTrivialModel("Lamp", mesh);
}

auto StatefulAssetBuilder::createLampMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = addTextureAsset("Lamp.png");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 32.0f;

    return addMaterialAsset({ambientColor, diffuseMap, specularMap, shininess});
}

auto StatefulAssetBuilder::createFlashlightModel()
    -> Model &
{
    auto const & shape = addShapeAsset(makeBox(NormalDirection::inbound, {0.3f, 0.1f, 0.1f}));

    auto const & material = createFlashlightMaterial();

    auto const & mesh = addMeshAsset({"Flashlight", shape, material});

    return createTrivialModel("Flashlight", mesh);
}

auto StatefulAssetBuilder::createFlashlightMaterial()
    -> Material &
{
    auto const ambientColor = glm::vec3{1.0f, 1.0f, 1.0f};

    auto const & texture = addTextureAsset("Flashlight.png");

    auto const & diffuseMap = texture;

    auto const & specularMap = texture;

    auto const shininess = 32.0f;

    return addMaterialAsset({ambientColor, diffuseMap, specularMap, shininess});
}

auto StatefulAssetBuilder::createTrivialModel(std::string name, Mesh const & mesh)
    -> Model &
{
    auto rootPart = ModelPart{"", {&mesh}, {}, glm::mat4{1.0f}};

    auto model = Model{std::move(name), "", std::move(rootPart)};

    return addModelAsset(std::move(model));
}

auto StatefulAssetBuilder::addShapeAsset(Shape shape)
    -> Shape &
{
    return repository.getAssets().shapes.emplace_back(std::move(shape));
}

auto StatefulAssetBuilder::addTextureAsset(std::string const & path)
    -> Texture &
{
    return repository.getAssets().textures.emplace_back(path);
}

auto StatefulAssetBuilder::addMaterialAsset(Material const & material)
    -> Material &
{
    return repository.getAssets().materials.emplace_back(material);
}

auto StatefulAssetBuilder::addMeshAsset(Mesh const & mesh)
    -> Mesh &
{
    return repository.getAssets().meshes.emplace_back(mesh);
}

auto StatefulAssetBuilder::addModelAsset(Model model)
    -> Model &
{
    return repository.getAssets().models.emplace_back(std::move(model));
}

} // unnamed namespace

auto AssetBuilder::build() const
    -> AssetRepository
{
    auto builder = StatefulAssetBuilder{};

    return builder.build();
}
