#pragma once

#include <filesystem>

namespace ape
{

class BlinnPhongShaderCollection;

class BlinnPhongShaderCollectionPopulator
{

public:

    explicit BlinnPhongShaderCollectionPopulator(BlinnPhongShaderCollection & collection);

    auto addAllShadersInFolder(std::filesystem::path const & folderPath) const
        -> void;

    auto addAllShaders() const
        -> void;

private:

    auto addShaderInFolder(std::filesystem::path const & folderPath) const
        -> void;

private:

    BlinnPhongShaderCollection * collection;

};

} // namespace ape
