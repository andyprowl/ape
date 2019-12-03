#pragma once

#include <Ape/Rendering/Lighting/PointLightViewTransformation.hpp>

#include <Glow/Shader/Uniform.hpp>

namespace glow
{

template<>
class Uniform<ape::PointLightViewTransformation>
{

public:

    using ValueType = ape::PointLightViewTransformation;

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : right{program, prefix + ".right"}
        , left{program, prefix + ".left"}
        , top{program, prefix + ".top"}
        , bottom{program, prefix + ".bottom"}
        , front{program, prefix + ".front"}
        , back{program, prefix + ".back"}
    {
    }

    auto set(ValueType const & lightView)
        -> void
    {
        right = lightView.right;

        left = lightView.left;

        top = lightView.top;

        bottom = lightView.bottom;

        front = lightView.front;

        back = lightView.back;
    }

    auto operator = (ValueType const & lightView)
        -> Uniform &
    {
        set(lightView);

        return *this;
    }

public:

    Uniform<glm::mat4> right;

    Uniform<glm::mat4> left;

    Uniform<glm::mat4> top;

    Uniform<glm::mat4> bottom;

    Uniform<glm::mat4> front;

    Uniform<glm::mat4> back;

};

} // namespace glow