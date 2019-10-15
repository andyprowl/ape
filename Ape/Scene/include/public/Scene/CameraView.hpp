#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class Camera;

class CameraView
{

public:

    class Placement
    {

    public:

        Placement(glm::vec3 const & position, glm::vec3 const & direction, glm::vec3 const & up)
            : position{position}
            , direction{direction}
            , up{up}
        {
        }

    public:

        glm::vec3 position;

        glm::vec3 direction;

        glm::vec3 up;

    };

public:

    CameraView(Placement const & placement, Camera & parent);

    auto getTransformation() const
        -> glm::mat4 const &;

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

    auto getParent() const
        -> Camera &;

private:

    friend class Camera;

private:

    auto makeView() const
        -> glm::mat4;

    auto recalculateViewAndNotifyParent()
        -> void;

    auto setParent(Camera & camera)
        -> void;

private:

    Placement placement;

    glm::mat4 transformation;

    Camera * parent;

};

} // namespace ape
