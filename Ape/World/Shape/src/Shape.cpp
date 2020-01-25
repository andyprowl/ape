#include <Ape/World/Shape/Shape.hpp>

#include <glad/glad.h>
#include <glm/geometric.hpp>

namespace ape
{

namespace
{

auto const minFloat = std::numeric_limits<float>::min();

auto const maxFloat = std::numeric_limits<float>::max();

auto makeAxisAlignedBoundingBox(std::vector<ShapeVertex> const & vertices)
    -> Box
{
    auto min = glm::vec3{maxFloat, maxFloat, maxFloat};
    
    auto max = glm::vec3{minFloat, minFloat, minFloat};

    for (auto const & vertex : vertices)
    {
        auto const & pos = vertex.position;

        min = {std::min(min.x, pos.x), std::min(min.y, pos.y), std::min(min.z, pos.z)};

        max = {std::max(max.x, pos.x), std::max(max.y, pos.y), std::max(max.z, pos.z)};
    }

    return {(min + max) / 2.0f, {max - min}};
}

auto makeBoundingSphere(Box const & box)
    -> Sphere
{
    auto const center = box.getCenter();

    auto const max = box.getCorner(Box::Corner::rightTopFront);

    auto const radius = glm::length(max - center);

    return {center, radius};
}

auto makeBoundingVolumes(std::vector<ShapeVertex> const & vertices)
    -> ShapeBounds
{
    auto const box = makeAxisAlignedBoundingBox(vertices);

    auto const sphere = makeBoundingSphere(box);

    return {box, sphere};
}

} // unnamed namespace

Shape::Shape(std::vector<ShapeVertex> vertices, std::vector<unsigned int> indices)
    : vertices{std::move(vertices)}
    , indices{std::move(indices)}
    , boundingVolumes{makeBoundingVolumes(this->vertices)}
{
}

auto Shape::getVertices() const 
    -> const std::vector<ShapeVertex> &
{
    return vertices;
}

auto Shape::getIndices() const 
    -> const std::vector<unsigned int> &
{
    return indices;
}

auto Shape::getBoundingVolumes() const
    -> ShapeBounds const &
{
    return boundingVolumes;
}

} // namespace ape
