#pragma once

#include <string>
#include <vector>

struct aiNode;
struct aiScene;

namespace ape
{

class AssetRepository;
class Mesh;
class Model;
class ModelPart;

class ModelPartImporter
{

public:

    explicit ModelPartImporter(AssetRepository & target);

    auto importRootPart(aiScene const & scene) const
        -> ModelPart;

private:

    auto importPart(aiNode const & node, aiScene const & scene) const
        -> ModelPart;

    auto importMeshes(aiNode const & node) const
        -> std::vector<Mesh const *>;

private:

    AssetRepository * target;

};

} // namespace ape
