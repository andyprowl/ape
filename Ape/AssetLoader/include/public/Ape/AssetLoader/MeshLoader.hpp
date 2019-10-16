#pragma once

#include <glm/vec2.hpp>

#include <vector>

struct aiMesh;
struct aiScene;

namespace ape
{

class AssetRepository;
class Shape;
class ShapeVertex;

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
        -> std::vector<ShapeVertex>;

    auto importTextureCoordinates(aiMesh const & mesh, int vertexIndex) const
        -> glm::vec2;

    auto importIndices(aiMesh const & mesh) const
        -> std::vector<unsigned int>;

private:

    AssetRepository * assets;

};

} // namespace ape
