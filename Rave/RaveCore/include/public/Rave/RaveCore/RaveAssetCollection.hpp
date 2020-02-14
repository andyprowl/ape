#pragma once

#include <Ape/World/AssetLoader/AssetCollection.hpp>
#include <Ape/World/AssetLoader/AssetLoader.hpp>

#include <filesystem>
#include <string>

namespace rave
{

class RaveAssetCollection : public ape::AssetCollection
{

public:

    explicit RaveAssetCollection(bool excludeSponza);

    auto getSimpleAssets()
        -> ape::AssetRepository &;

    auto getNanosuitModel()
        -> ape::Model &;

    auto getDragonModel()
        -> ape::Model &;

    auto getSpaceshipModel()
        -> ape::Model &;

    auto getDynoModel()
        -> ape::Model &;

    auto getCastleModel()
        -> ape::Model &;

    auto getTavernModel()
        -> ape::Model &;

    auto getHouseModel()
        -> ape::Model &;

    auto getCottageModel()
        -> ape::Model &;

    auto getBridgeModel()
        -> ape::Model &;

    auto getLibertyStatueModel()
        -> ape::Model &;

    auto getSponzaModel()
        -> ape::Model &;

private:

    void createSimpleAssets();

    void loadAssets(std::filesystem::path const & path, std::string modelName);

private:

    ape::AssetLoader loader;

};

} // namespace rave
