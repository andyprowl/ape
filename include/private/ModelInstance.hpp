#pragma once

#include "ModelPartInstance.hpp"

#include <vector>

class Model;

class ModelInstance
{

public:

    explicit ModelInstance(Model const & model);

    auto getModel() const
        -> Model const &;

    auto getPart(int index)
        -> ModelPartInstance &;

    auto getPart(int index) const
        -> ModelPartInstance const &;

    auto getNumOfParts() const
        -> int;

private:

    Model const * model;

    std::vector<ModelPartInstance> parts;

};

auto getRootPart(ModelInstance & model)
    -> ModelPartInstance &;

auto getRootPart(ModelInstance const & model)
    -> ModelPartInstance const &;

auto getPosition(ModelInstance const & model)
    -> glm::vec3;

auto setPosition(ModelInstance & model, glm::vec3 const & newPosition)
    -> void;

auto getTransformation(ModelInstance const & model)
    -> glm::mat4 const &;

auto setTransformation(ModelInstance & model, glm::mat4 const & newTransformation)
    -> void;

auto scaleUniformly(ModelInstance & model, float factor)
    -> void;
