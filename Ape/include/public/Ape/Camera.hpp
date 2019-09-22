#pragma once

#include <Ape/Signal.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class Mesh;
class ShaderProgram;
class Scene;

class Camera
{

public:

    using TransformEventSignature = auto (glm::mat4 const & t) -> void;

public:

    Camera(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        glm::vec3 const & up,
        float fieldOfView,
        float aspectRatio);

    auto getView() const
        -> glm::mat4;

    auto getProjection() const
        -> glm::mat4;

    auto getTransformation() const
        -> glm::mat4;

    auto getPosition() const
        -> glm::vec3;

    auto setPosition(glm::vec3 const & newPosition)
        -> void;

    auto getDirection() const
        -> glm::vec3;

    auto setDirection(glm::vec3 const & newDirection)
        -> void;

    auto getUp() const
        -> glm::vec3;

    auto getFieldOfView() const
        -> float;

    auto setFieldOfView(float newFieldOfView)
        -> void;

    auto getAspectRatio() const
        -> float;

    auto setAspectRatio(float newAspectRatio)
        -> void;

public:

    mutable Signal<TransformEventSignature> onViewChanged;

    mutable Signal<TransformEventSignature> onProjectionChanged;

    mutable Signal<TransformEventSignature> onTransformationChanged;

private:

    auto makeView() const
        -> glm::mat4;

    auto makeProjection() const
        -> glm::mat4;

    auto setViewAndFireEvent(glm::mat4 const & newView)
        -> void;

    auto setProjectionAndFireEvent(glm::mat4 const & newProjection)
        -> void;

private:

    glm::vec3 position;

    glm::vec3 direction;

    glm::vec3 up;

    float fieldOfView;

    float aspectRatio;

    glm::mat4 view;

    glm::mat4 projection;

    glm::mat4 transformation;

};

} // namespace ape
