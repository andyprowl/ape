#pragma once

#include "GLFW.hpp"
#include "Transformable.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

class Mesh;
class ShaderProgram;
class Scene;

class Camera : public Transformable
{

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

private:

    // virtual (from Transformable)
    auto onContextTransformationChanged(glm::mat4 const & contextTransformation)
        -> void override;

    auto makeView() const
        -> glm::mat4;

    auto makeProjection() const
        -> glm::mat4;

    auto updateView()
        -> void;

    auto updateProjection()
        -> void;
    
    auto updateTransformation()
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
