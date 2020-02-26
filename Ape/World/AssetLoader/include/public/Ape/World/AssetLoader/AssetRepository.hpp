#pragma once

#include <Ape/World/Model/Material.hpp>
#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/Model.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/Texture/Texture2d.hpp>

#include <memory>
#include <vector>

namespace glow
{

class Texture2d;

} // namespace glow

namespace ape
{

class AssetRepository
{

public:

    std::vector<Shape> shapes;

    std::vector<glow::Texture2d> textures;

    std::vector<Material> materials;

    std::vector<Mesh> meshes;

    std::vector<Model> models;

};

} // namespace ape
