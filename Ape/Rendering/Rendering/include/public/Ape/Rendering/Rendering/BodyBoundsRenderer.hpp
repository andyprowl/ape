#pragma once

#include <Ape/Rendering/Skybox/PlainCube.hpp>
#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyBoundsShaderProgram;
class BodyPart;
class BodyPartMesh;
class Camera;

class BodyBoundsRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    explicit BodyBoundsRenderer(BodyBoundsShaderProgram & shader);

    auto render(BodySetView const & bodies, Camera const & camera) const
        -> void;

private:

    auto renderBody(
        Body const & body,
        Camera const & camera) const
        -> void;

    auto renderBodyPart(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation) const
        -> void;

    auto renderMesh(
        BodyPartMesh const & mesh,
        glm::mat4 const & cameraTransformation,
        glm::mat4 const & partTransformation) const
        -> void;

    auto drawBox() const
        -> void;

private:

    BodyBoundsShaderProgram * shader;

    PlainCube cubeShape;

};

} // namespace ape
