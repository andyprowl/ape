#include <Asset/Mesh.hpp>

namespace ape
{

Mesh::Mesh(std::string name, Shape const & shape, Material const & material)
    : name{std::move(name)}
    , shape{&shape}
    , material{&material}
{
}

auto Mesh::getName() const
    -> std::string
{
    return name;
}

auto Mesh::getShape() const
    -> Shape const &
{
    return *shape;
}

auto Mesh::getMaterial() const
    -> Material const &
{
    return *material;
}

} // namespace ape
