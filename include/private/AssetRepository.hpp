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

    class Assets
    {
    
    public:

        std::vector<Shape> shapes;

        std::vector<Texture> textures;

        std::vector<Material> materials;

        std::vector<Mesh> meshes;

        std::vector<Model> models;
    
    };

public:

    AssetRepository()
        : assets{std::make_unique<Assets>()}
    {
    }

    auto getAssets()
        -> Assets &
    {
        return *assets;
    }

    auto getAssets() const
        -> Assets const &
    {
        return *assets;
    }

private:

    std::unique_ptr<Assets> assets;

};
