#pragma once

#include <Ape/Camera.hpp>
#include <Ape/Uniform.hpp>

template<>
class Uniform<Camera>
{

public:

    Uniform(ShaderProgram const & program, std::string const & prefix)
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
