#pragma once

#include <Engine/ShapeRenderer.hpp>

#include <GpuResource/VertexArrayObject.hpp>

#include <vector>

namespace ape
{

class AssetRepository;
class Shape;

class ShapeArrayObjectRenderer : public ShapeRenderer
{

public:

    explicit ShapeArrayObjectRenderer(std::vector<AssetRepository const *> assets);

    // virtual (from ShapeRenderer)
    auto render(Shape const & shape) const
        -> void override;

private:

    auto makeShapeArrayObjects() const
        -> std::vector<VertexArrayObject>;
    
    auto setupArrayObjectsForShapes(
        AssetRepository const & repository,
        std::vector<VertexArrayObject> & destination) const
        -> void;

    auto getArrayObjectForShape(Shape const & shape) const
        -> const VertexArrayObject &;

private:

    std::vector<AssetRepository const *> assets;

    std::vector<VertexArrayObject> shapeArrayObjects;

};

} // namespace ape
