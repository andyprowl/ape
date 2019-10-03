#pragma once

#include <Asset/ModelPart.hpp>

#include <filesystem>
#include <memory>

namespace ape
{

class Model
{

public:

    Model(ModelPart rootPart, std::string name, std::filesystem::path source);

    auto getRootPart() const
        -> const ModelPart &;

    auto getName() const
        -> std::string;

    auto getSource() const
        -> std::filesystem::path;

private:

    ModelPart rootPart;

    std::string name;

    std::filesystem::path source;

};

} // namespace ape
