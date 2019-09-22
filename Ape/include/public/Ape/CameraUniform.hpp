#pragma once

#include <Ape/Camera.hpp>
#include <Ape/Uniform.hpp>

namespace ape
{

template<>
class Uniform<Camera>
{

public:

    Uniform(ShaderProgram & program, std::string const & prefix)
        : cameraPosition{program, prefix + ".position"}
        , cameraTransformation{program, prefix + ".transform"}
    {
    }

    auto set(const Camera & camera)
        -> void
    {
        cameraPosition = camera.getPosition();

        cameraTransformation = camera.getTransformation();
    }

public:

    Uniform<glm::vec3> cameraPosition;

    Uniform<glm::mat4> cameraTransformation;
    
};

} // namespace ape
