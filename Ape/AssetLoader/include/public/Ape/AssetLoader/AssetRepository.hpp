#pragma once

#include <Ape/Model/Material.hpp>
#include <Ape/Model/Mesh.hpp>
#include <Ape/Model/Model.hpp>
#include <Ape/Model/Shape.hpp>

#include <Ape/GpuResource/Texture.hpp>

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
