#include "Model.hpp"

Model::Model(std::string source, std::string name, ModelPart rootPart)
    : source{std::move(source)}
    , name{std::move(name)}
    , rootPart{std::move(rootPart)}
{
}

auto Model::getName() const
    -> std::string
{
    return name;
}

auto Model::getSource() const
    -> std::string
{
    return source;
}

auto Model::getRootPart() const
    -> const ModelPart &
{
    return rootPart;
}
