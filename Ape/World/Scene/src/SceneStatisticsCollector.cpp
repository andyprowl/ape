#include <Ape/World/Scene/SceneStatisticsCollector.hpp>

#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Scene.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPartMesh.hpp>
#include <Ape/World/Shape/Shape.hpp>

namespace ape
{

namespace
{

auto getNumOfTrianglesInMesh(BodyPartMesh const & partMesh)
    -> int
{
    auto const & mesh = partMesh.getModel();

    auto const & shape = mesh.getShape();

    const auto & indices = shape.getIndices();

    return static_cast<int>(indices.size()) / 3;
}

auto getNumOfTrianglesInBodyPart(BodyPart const & part)
    -> int
{
    auto numOfTriangles = 0;

    for (auto const & partMesh : part.getMeshes())
    {
        numOfTriangles += getNumOfTrianglesInMesh(partMesh);
    }

    return numOfTriangles;
}

auto getNumOfTrianglesInBody(Body const & body)
    -> int
{
    auto numOfTriangles = 0;

    for (auto const & part : body.getParts())
    {
        numOfTriangles += getNumOfTrianglesInBodyPart(part);
    }

    return numOfTriangles;
}

} // unnamed namespace

SceneStatisticsCollector::SceneStatisticsCollector(Scene const & scene)
    : scene{&scene}
{
}

auto SceneStatisticsCollector::getTotalNumOfTriangles() const
    -> int
{
    auto numOfTriangles = 0;

    for (auto const & body : scene->getBodies())
    {
        numOfTriangles = getNumOfTrianglesInBody(body);
    }

    return numOfTriangles;
}

} // namespace ape
