#pragma once

#include <string>
#include <vector>

struct aiNode;
struct aiScene;

class AssetRepository;
class Mesh;
class ModelPart;

class ModelLoader
{

public:

    explicit ModelLoader(AssetRepository & target);

    auto load(aiScene const & scene) const
        -> void;

private:

    auto loadModelPart(aiNode const & node, aiScene const & scene) const
        -> ModelPart;

    auto importMeshes(aiNode const & node) const
        -> std::vector<Mesh const *>;

private:

    AssetRepository * target;

};
