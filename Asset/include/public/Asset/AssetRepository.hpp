#pragma once

#include <Asset/Material.hpp>
#include <Asset/Mesh.hpp>
#include <Asset/Model.hpp>
#include <Asset/Shape.hpp>

#include <Glow/Texture.hpp>

#include <memory>
#include <vector>

namespace ape
{

class AssetRepository
{

public:

    std::vector<Shape> shapes;

    std::vector<Texture> textures;

    std::vector<Material> materials;

    std::vector<Mesh> meshes;

    std::vector<Model> models;

};

} // namespace ape
