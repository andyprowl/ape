#include <Core/StandardShaderProgram.hpp>

namespace ape
{

StandardShaderProgram::StandardShaderProgram()
    : ShaderProgram{"Vertex.glsl", "Fragment.glsl"}
    , camera{*this, "camera"}
    , lighting{*this, "lighting"}
    , modelTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , materialAmbient{*this, "material.ambient"}
    , materialShininess{*this, "material.shininess"}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
{
    use();

    getUniform<int>("material.diffuse") = 0;

    getUniform<int>("material.specular") = 1;
}

} // namespace ape
