#include <Ape/StandardShaderProgram.hpp>

StandardShaderProgram::StandardShaderProgram()
    : ShaderProgram{"Vertex.glsl", "Fragment.glsl"}
    , camera{*this, "camera"}
    , lighting{*this, "lighting"}
    , modelTransformation{*this, "model.transform"}
    , normalMatrix{*this, "model.normal"}
    , materialAmbient{*this, "material.ambient"}
    , materialShininess{*this, "material.shininess"}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
{
    use();

    getUniform<int>("material.diffuse") = 0;

    getUniform<int>("material.specular") = 1;
}
