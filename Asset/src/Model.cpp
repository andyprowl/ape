#include <Asset/Model.hpp>

namespace ape
{

Model::Model(ModelPart rootPart, std::string source, std::string name)
    : rootPart{std::move(rootPart)}
    , name{std::move(name)}
    , source{std::move(source)}
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

auto Model::getSource() const
    -> std::string
{
    return source;
}

} // namespace ape
