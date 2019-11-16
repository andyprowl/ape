#include <Ape/World/Shape/Box.hpp>

namespace ape
{

namespace
{

auto makeCorners(
    glm::vec3 const & center,
    glm::vec3 const & size,
    glm::mat3 const & rotationAroundCenter)
    -> std::array<glm::vec3, 8u>
{
    return {{
        center + rotationAroundCenter * glm::vec3{+1.0f, +1.0f, +1.0f} * (size / 2.0f), // RTF
        center + rotationAroundCenter * glm::vec3{+1.0f, +1.0f, -1.0f} * (size / 2.0f), // RTB
        center + rotationAroundCenter * glm::vec3{+1.0f, -1.0f, +1.0f} * (size / 2.0f), // RBF
        center + rotationAroundCenter * glm::vec3{+1.0f, -1.0f, -1.0f} * (size / 2.0f), // RBB
        center + rotationAroundCenter * glm::vec3{-1.0f, +1.0f, +1.0f} * (size / 2.0f), // LTF
        center + rotationAroundCenter * glm::vec3{-1.0f, +1.0f, -1.0f} * (size / 2.0f), // LTB
        center + rotationAroundCenter * glm::vec3{-1.0f, -1.0f, +1.0f} * (size / 2.0f), // LBF
        center + rotationAroundCenter * glm::vec3{-1.0f, -1.0f, -1.0f} * (size / 2.0f), // LBB
        }};
}

} // unnamed namespace

Box::Box(glm::vec3 const & center, glm::vec3 const & size, glm::mat3 const & rotationAroundCenter)
    : center{center}
    , corners{makeCorners(center, size, rotationAroundCenter)}
{
}

auto Box::transform(glm::mat4 const & transformation)
    -> void
{
    center = glm::vec3{transformation * glm::vec4{center, 1.0f}};

    for (auto & corner : corners)
    {
        corner = glm::vec3{transformation * glm::vec4{corner, 1.0f}};
    }
}

auto Box::translate(glm::vec3 const & offset)
    -> void
{
    center += offset;

    for (auto & corner : corners)
    {
        corner += offset;
    }
}

auto Box::scale(float const factor)
    -> void
{
    for (auto & corner : corners)
    {
        corner = center + (corner - center) * factor;
    }
}

} // namespace ape
