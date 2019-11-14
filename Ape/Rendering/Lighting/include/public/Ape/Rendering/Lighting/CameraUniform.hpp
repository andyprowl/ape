#pragma once

#include <Glow/Shader/Uniform.hpp>

#include <Ape/World/Scene/Camera.hpp>

namespace glow
{

template<>
class Uniform<ape::Camera>
{

public:

    Uniform(ShaderProgram & program, std::string prefix)
        : cameraPosition{program, std::move(prefix) + ".position"}
    {
    }

    auto set(const ape::Camera & camera)
        -> void
    {
        cameraPosition = camera.getView().getPosition();
    }

public:

    Uniform<glm::vec3> cameraPosition;
    
};

} // namespace glow
