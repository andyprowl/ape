#pragma once

#include <Ape/World/Model/ModelPart.hpp>

#include <memory>

namespace ape
{

class Model
{

public:

    Model(ModelPart rootPart, std::string namee);

    auto getRootPart() const
        -> const ModelPart &;

    auto getName() const
        -> std::string;

private:

    ModelPart rootPart;

    std::string name;

};

} // namespace ape
