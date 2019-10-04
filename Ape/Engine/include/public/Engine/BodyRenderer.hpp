#pragma once

#include <vector>

namespace ape
{

class Body;
class BodyPart;
class Mesh;
class ShapeRenderer;

class BodyRenderer
{

public:

    explicit BodyRenderer(ShapeRenderer const & shapeRenderer);

    auto render(std::vector<Body *> const & bodies) const
        -> void;

private:

    auto renderBody(Body const & body) const
        -> void;

    auto renderBodyPart(BodyPart const & part) const
        -> void;

    auto renderMesh(Mesh const & mesh) const
        -> void;

    virtual auto onBeforeBodyRendering(Body const & body) const
        -> void;

    virtual auto onAfterBodyRendering(Body const & body) const
        -> void;

    virtual auto onBeforePartRendering(BodyPart const & part) const
        -> void;

    virtual auto onAfterPartRendering(BodyPart const & part) const
        -> void;

    virtual auto onBeforeMeshRendering(Mesh const & mesh) const
        -> void;

    virtual auto onAfterMeshRendering(Mesh const & mesh) const
        -> void;

private:

    ShapeRenderer const * shapeRenderer;

};

} // namespace ape
