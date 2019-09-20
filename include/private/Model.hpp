#pragma once

#include "ModelPart.hpp"

#include <memory>

class Model
{

public:

    Model(std::string name, std::string source, ModelPart rootPart);

    auto getName() const
        -> std::string;

    auto getSource() const
        -> std::string;

    auto getRootPart() const
        -> const ModelPart &;

private:

    std::string name;

    std::string source;

    ModelPart rootPart;

};
