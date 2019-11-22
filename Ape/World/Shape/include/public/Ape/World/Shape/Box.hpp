#pragma once

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <array>

namespace ape
{

class Box
{

public:

    enum class Corner
    {
        rightTopFront,
        rightTopBack,
        rightBottomFront,
        rightBottomBack,
        leftTopFront,
        leftTopBack,
        leftBottomFront,
        leftBottomBack};

public:

    Box(
        glm::vec3 const & center,
        glm::vec3 const & extents,
        glm::mat3 const & rotationAroundCenter = glm::mat3{1.0f});

    auto getCenter() const
        -> glm::vec3 const &
    {
        return center;
    }

    auto getCorners() const
        -> std::array<glm::vec3, 8u> const &
    {
        return corners;
    }

    auto getCorner(Corner const corner) const
        -> glm::vec3 const &
    {
        return corners[static_cast<std::size_t>(corner)];
    }

    auto transform(glm::mat4 const & transformation)
        -> void;

    auto translate(glm::vec3 const & offset)
        -> void;

    auto scale(float factor)
        -> void;

private:

    glm::vec3 center;

    std::array<glm::vec3, 8u> corners;

};

} // namespace ape
