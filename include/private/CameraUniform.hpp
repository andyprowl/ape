#pragma once

#include "Camera.hpp"
#include "Uniform.hpp"

class CameraUniform
{

public:

    CameraUniform(ShaderProgram const & program, std::string const & prefix)
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
