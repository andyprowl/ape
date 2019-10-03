#include <Asset/Model.hpp>

namespace ape
{

Model::Model(ModelPart rootPart, std::string name, std::filesystem::path source)
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
    -> std::filesystem::path
{
    return source;
}

} // namespace ape
