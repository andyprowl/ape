#pragma once

#include "BodyPart.hpp"

#include <vector>

class Model;

class Body
{

public:

    explicit Body(Model const & model);

    auto getModel() const
        -> Model const &;

    auto getPart(int index)
        -> BodyPart &;

    auto getPart(int index) const
        -> BodyPart const &;

    auto getNumOfParts() const
        -> int;

private:

    Model const * model;

    std::vector<BodyPart> parts;

};

auto getRootPart(Body & model)
    -> BodyPart &;

auto getRootPart(Body const & model)
    -> BodyPart const &;

auto getPosition(Body const & model)
    -> glm::vec3;

auto setPosition(Body & model, glm::vec3 const & newPosition)
    -> void;

auto getTransformation(Body const & model)
    -> glm::mat4 const &;

auto setTransformation(Body & model, glm::mat4 const & newTransformation)
    -> void;

auto scaleUniformly(Body & model, float factor)
    -> void;
