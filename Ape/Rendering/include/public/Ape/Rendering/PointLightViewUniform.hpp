#pragma once

#include <Ape/Rendering/PointLightView.hpp>

#include <Ape/GpuResource/Uniform.hpp>

namespace ape
{

template<>
class Uniform<PointLightView>
{

public:

    using ValueType = PointLightView;

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

} // namespace ape
