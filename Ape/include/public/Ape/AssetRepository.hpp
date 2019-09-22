#pragma once

#include <Ape/Material.hpp>
#include <Ape/Mesh.hpp>
#include <Ape/Model.hpp>
#include <Ape/Shape.hpp>
#include <Ape/Texture.hpp>

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
