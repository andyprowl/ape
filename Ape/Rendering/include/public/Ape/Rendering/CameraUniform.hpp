#pragma once

#include <Ape/GpuResource/Uniform.hpp>

#include <Ape/Scene/Camera.hpp>

namespace ape
{

template<>
class Uniform<Camera>
{

public:

    Uniform(ShaderProgram & program, std::string prefix)
        : cameraPosition{program, std::move(prefix) + ".position"}
    {
    }

    auto set(const Camera & camera)
        -> void
    {
        cameraPosition = camera.getView().getPosition();
    }

public:

    Uniform<glm::vec3> cameraPosition;
    
};

} // namespace ape