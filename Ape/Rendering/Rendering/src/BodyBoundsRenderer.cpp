#include <Ape/Rendering/Rendering/BodyBoundsRenderer.hpp>

#include <Ape/Rendering/Rendering/BodyBoundsShaderProgram.hpp>

#include <Ape/World/Model/Mesh.hpp>
#include <Ape/World/Model/Model.hpp>
#include <Ape/World/Model/ModelPart.hpp>
#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/BodyPart.hpp>
#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Shape.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>

namespace ape
{

namespace
{

auto getUnitCubeToModelBoundingBoxTransformation(BodyPartMesh const & mesh)
    -> glm::mat4
{
    auto const & modelBoundingBox = mesh.getModel().getShape().getBoundingVolumes().getBox();

    auto const & leftBottomBack = modelBoundingBox.getCorner(Box::Corner::leftBottomBack);

    auto const & rightTopFront = modelBoundingBox.getCorner(Box::Corner::rightTopFront);

    auto const extents = (rightTopFront - leftBottomBack);

    auto const scaling = glm::scale(glm::mat4{1.0f}, extents);

    auto const translation = glm::translate(glm::mat4{1.0f}, modelBoundingBox.getCenter());

    return (translation * scaling);
}

auto isBoundingBoxCornerConsistent(
    Box const & boundingBox,
    Box::Corner const corner,
    glm::vec3 const & unitCubeCorner,
    glm::mat4 const & unitCubeTransformation)
    -> bool
{
    auto const & actualCorner = boundingBox.getCorner(corner);

    auto const drawnCorner = glm::vec3{unitCubeTransformation * glm::vec4{unitCubeCorner, 1.0}};

    auto const error = glm::distance(drawnCorner, actualCorner);

    auto constexpr epsilon = 1.0e-5f;

    return (error <= epsilon);
}

auto isBoundingBoxConsistent(BodyPartMesh const & mesh, glm::mat4 const & unitCubeTransformation)
    -> bool
{
    auto isCornerConsistent = [&] (Box::Corner const corner, glm::vec3 const & unitCubeCorner)
    {
        auto const & boundingBox = mesh.getBoundingVolumes().getBox();

        return isBoundingBoxCornerConsistent(
            boundingBox,
            corner,
            unitCubeCorner,
            unitCubeTransformation);
    };

    return isCornerConsistent(Box::Corner::rightTopFront, {+0.5f, +0.5f, +0.5f})
        && isCornerConsistent(Box::Corner::rightTopBack, {+0.5f, +0.5f, -0.5f})
        && isCornerConsistent(Box::Corner::rightBottomFront, {0.5f, -0.5f, +0.5f})
        && isCornerConsistent(Box::Corner::rightBottomBack, {0.5f, -0.5f, -0.5f})
        && isCornerConsistent(Box::Corner::leftTopFront, {-0.5f, +0.5f, +0.5f})
        && isCornerConsistent(Box::Corner::leftTopBack, {-0.5f, +0.5f, -0.5f})
        && isCornerConsistent(Box::Corner::leftBottomFront, {-0.5f, -0.5f, +0.5f})
        && isCornerConsistent(Box::Corner::leftBottomBack, {-0.5f, -0.5f, -0.5f});
}

} // unnamed namespace

BodyBoundsRenderer::BodyBoundsRenderer(BodyBoundsShaderProgram & shader)
    : shader{&shader}
{
}

auto BodyBoundsRenderer::render(BodySetView const & bodies, Camera const & camera) const
    -> void
{
    glDisable(GL_CULL_FACE);

    auto const shaderBinder = bind(*shader);

    for (auto const & body : bodies)
    {
        renderBody(body, camera);
    }

    glEnable(GL_CULL_FACE);
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

        renderMesh(bodyMesh, cameraTransformation, partTransformation);
    }
}

auto BodyBoundsRenderer::renderMesh(
    BodyPartMesh const & mesh,
    glm::mat4 const & cameraTransformation,
    glm::mat4 const & partTransformation) const
    -> void
{
    auto const cubeToModelBoundingBox = getUnitCubeToModelBoundingBoxTransformation(mesh);

    auto const cubeToBoundingBox = partTransformation * cubeToModelBoundingBox;

    assert(isBoundingBoxConsistent(mesh, cubeToBoundingBox));

    shader->transformation.set(cameraTransformation * cubeToBoundingBox);

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
