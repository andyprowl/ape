#pragma once

#include "AssetRepository.hpp"

struct aiMesh;
struct aiScene;

class MeshLoader
{

public:

    explicit MeshLoader(AssetRepository::Assets & assets);

    auto load(aiScene const & scene)
        -> void;

private:

    auto importMesh(aiMesh const & mesh)
        -> void;

    auto importShape(aiMesh const & mesh)
        -> Shape const &;

    auto importVertices(aiMesh const & mesh)
        -> std::vector<Vertex>;

    auto importIndices(aiMesh const & mesh)
        -> std::vector<unsigned int>;

private:

    AssetRepository::Assets * assets;

};
