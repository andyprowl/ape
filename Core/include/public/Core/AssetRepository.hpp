#pragma once

#include <Core/Material.hpp>
#include <Core/Mesh.hpp>
#include <Core/Model.hpp>
#include <Core/Shape.hpp>

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
