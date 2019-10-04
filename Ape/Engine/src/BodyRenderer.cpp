#include <Engine/BodyRenderer.hpp>

#include <Engine/ShapeRenderer.hpp>

#include <Scene/Body.hpp>

#include <Asset/Mesh.hpp>
#include <Asset/Model.hpp>
#include <Asset/Shape.hpp>

namespace ape
{

BodyRenderer::BodyRenderer(ShapeRenderer const & shapeRenderer)
    : shapeRenderer{&shapeRenderer}
{
}

auto BodyRenderer::render(std::vector<Body *> const & bodies) const
    -> void
{
    for (auto const body : bodies)
    {
        onBeforeBodyRendering(*body);

        renderBody(*body);

        onAfterBodyRendering(*body);
    }
}

auto BodyRenderer::renderBody(Body const & body) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        onBeforePartRendering(part);

        renderBodyPart(part);

        onAfterPartRendering(part);
    }
}

auto BodyRenderer::renderBodyPart(BodyPart const & part) const
    -> void
{
    for (auto const mesh : part.getModel().getMeshes())
    {
        onBeforeMeshRendering(*mesh);

        renderMesh(*mesh);

        onAfterMeshRendering(*mesh);
    }
}

auto BodyRenderer::renderMesh(Mesh const & mesh) const
    -> void
{
    auto const & shape = mesh.getShape();

    shapeRenderer->render(shape);
}

// virtual
auto BodyRenderer::onBeforeBodyRendering(Body const & /*body*/) const
    -> void
{
}

// virtual
auto BodyRenderer::onAfterBodyRendering(Body const & /*body*/) const
    -> void
{
}

// virtual
auto BodyRenderer::onBeforePartRendering(BodyPart const & /*part*/) const
    -> void
{
}

// virtual
auto BodyRenderer::onAfterPartRendering(BodyPart const & /*part*/) const
    -> void
{
}

// virtual
auto BodyRenderer::onBeforeMeshRendering(Mesh const & /*mesh*/) const
    -> void
{
}

// virtual
auto BodyRenderer::onAfterMeshRendering(Mesh const & /*mesh*/) const
    -> void
{
}

} // namespace ape
