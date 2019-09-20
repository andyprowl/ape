#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Shape.hpp"
#include "Texture.hpp"

#include <memory>
#include <vector>

class AssetRepository
{

public:

    std::vector<Shape> shapes;

    std::vector<Texture> textures;

    std::vector<Material> materials;

    std::vector<Mesh> meshes;

    std::vector<Model> models;

};
