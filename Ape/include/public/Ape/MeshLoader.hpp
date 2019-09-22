#pragma once

#include "AssetRepository.hpp"

struct aiMesh;
struct aiScene;

namespace ape
{

class MeshLoader
{

public:

    explicit MeshLoader(AssetRepository & assets);

    auto load(aiScene const & scene) const
        -> void;

private:

    auto importMesh(aiMesh const & mesh) const
        -> void;

    auto importShape(aiMesh const & mesh) const
        -> Shape const &;

    auto importVertices(aiMesh const & mesh) const
        -> std::vector<Vertex>;

    auto importIndices(aiMesh const & mesh) const
        -> std::vector<unsigned int>;

private:

    AssetRepository * assets;

};

} // namespace ape
