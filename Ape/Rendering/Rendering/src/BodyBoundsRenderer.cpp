#include <Ape/Rendering/Rendering/BodyBoundsRenderer.hpp>

#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

BodyBoundsRenderer::BodyBoundsRenderer(BodyBoundsShaderProgram & shader)
    : shader{&shader}
{
}

auto BodyBoundsRenderer::render(BodySetView const & bodies, Camera const & camera) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    for (auto const & body : bodies)
    {
        renderBody(body, camera);
    }
}

auto BodyBoundsRenderer::renderBody(
    Body const & body,
    Camera const & camera) const
    -> void
{
    auto const cameraTransformation = camera.getTransformation();

    for (auto const & part : body.getParts())
    {
        renderBodyPart(part, cameraTransformation);
    }
}

auto BodyBoundsRenderer::renderBodyPart(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    for (auto const & bodyMesh : part.getMeshes())
    {
        auto const & partTransformation = part.getWorldTransformation();

        auto const transformation = cameraTransformation * partTransformation;

        renderMesh(bodyMesh, transformation);
    }
}

auto BodyBoundsRenderer::renderMesh(
    BodyPartMesh const & mesh,
    glm::mat4 const & transformation) const
    -> void
{
    auto const & modelBoundingBox = mesh.getModel().getShape().getBoundingVolumes().getBox();

    auto const & leftBottomBack = modelBoundingBox.getCorner(Box::Corner::leftBottomBack);

    auto const & rightTopFront = modelBoundingBox.getCorner(Box::Corner::rightTopFront);

    auto const extents = (rightTopFront - leftBottomBack);

    auto const scaling = glm::scale(glm::mat4{1.0f}, extents);

    shader->transformation = transformation * scaling;

    drawBox();
}

auto BodyBoundsRenderer::drawBox() const
    -> void
{
    auto const arrayObjectBinder = glow::bind(cubeShape.getArrayObject());

    auto const numOfVertices = cubeShape.getNumOfVertices();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

} // namespace ape
