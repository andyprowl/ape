#pragma once

#include <Ape/World/AssetLoader/AssetRepository.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace ape
{

class AssetRepositoryNameNotUnique : public std::logic_error
{

public:

    explicit AssetRepositoryNameNotUnique(std::string name)
        : logic_error{
            "An asset repository with name " +
            std::move(name) +
            " was already registered"}
    {
    }

};

class AssetCollection
{

public:

    auto addAssets(std::string name, AssetRepository repository)
        -> void;

    auto getAssets(std::string const &  name)
        -> AssetRepository &;

    auto getShapes() const
        -> std::vector<Shape *> const &;

private:

    auto collectShapes(AssetRepository & repository)
        -> void;

private:

    std::unordered_map<std::string, AssetRepository> assetMap;

    std::vector<Shape *> shapes;

};

} // namespace ape
