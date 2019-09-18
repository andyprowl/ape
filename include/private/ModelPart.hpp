#pragma once

#include "Mesh.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

class ModelPart
{

public:

    

private:

    glm::mat4 transformation;

    std::vector<Mesh *> meshes;

    std::vector<ModelPart> childParts;

};
