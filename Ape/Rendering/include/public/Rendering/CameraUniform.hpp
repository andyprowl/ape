#pragma once

#include <Scene/Camera.hpp>

#include <GpuResource/Uniform.hpp>

namespace ape
{

template<>
class Uniform<Camera>
{

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : cameraPosition{program, prefix + ".position"}
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
