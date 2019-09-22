#pragma once

#include <Ape/ModelPart.hpp>

#include <memory>

class Model
{

public:

    Model(ModelPart rootPart, std::string name, std::string source);

    auto getRootPart() const
        -> const ModelPart &;

    auto getName() const
        -> std::string;

    auto getSource() const
        -> std::string;

private:

    ModelPart rootPart;

    std::string name;

    std::string source;

};
