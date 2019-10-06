#pragma once

#include <Model/Material.hpp>
#include <Model/Mesh.hpp>
#include <Model/Model.hpp>
#include <Model/Shape.hpp>

#include <GpuResource/Texture.hpp>

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
