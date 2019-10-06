#include <Model/Model.hpp>

namespace ape
{

Model::Model(ModelPart rootPart, std::string name)
    : rootPart{std::move(rootPart)}
    , name{std::move(name)}
{
}

auto Model::getRootPart() const
    -> const ModelPart &
{
    return rootPart;
}

auto Model::getName() const
    -> std::string
{
    return name;
}

} // namespace ape
