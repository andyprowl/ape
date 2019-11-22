#pragma once

#include <glm/mat4x4.hpp>

namespace ape
{

class PointLightViewTransformation
{

public:

    PointLightViewTransformation(
        glm::mat4 const & right,
        glm::mat4 const & left,
        glm::mat4 const & top,
        glm::mat4 const & bottom,
        glm::mat4 const & front,
        glm::mat4 const & back)
        : right{right}
        , left{left}
        , top{top}
        , bottom{bottom}
        , front{front}
        , back{back}
    {
    }

public:

    glm::mat4 right;

    glm::mat4 left;

    glm::mat4 top;

    glm::mat4 bottom;

    glm::mat4 front;

    glm::mat4 back;

};

inline auto operator * (PointLightViewTransformation const & v, glm::mat4 const & m)
    -> PointLightViewTransformation
{
    return {v.right * m, v.left * m, v.top * m, v.bottom * m, v.front * m, v.back * m};
}

} // namespace ape
