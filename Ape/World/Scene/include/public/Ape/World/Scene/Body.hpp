#pragma once

#include <Ape/World/Scene/BodyPart.hpp>

#include <Basix/Range/ContainerView.hpp>
#include <Basix/Signal/Signal.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace ape
{

class Model;

class Body
{

public:

    using PartContainer = std::vector<BodyPart>;

public:
    
    explicit Body(Model const & model);

    Body(Model const & model, std::string name);

    Body(Body && rhs) noexcept;

    auto operator = (Body && rhs) noexcept
        -> Body &;

    ~Body() = default;

    auto getName() const
        -> std::string_view;

    auto getModel() const
        -> Model const &;

    auto getNumOfParts() const
        -> int;

    auto getPart(int index)
        -> BodyPart &;

    auto getPart(int index) const
        -> BodyPart const &;

    auto getParts()
        -> basix::ContainerView<PartContainer>;

    auto getParts() const
        -> basix::ContainerView<PartContainer const>;

public:

    mutable basix::Signal<auto (BodyPart const & source) -> void> onLocalTransformationChanged;

private:

    auto connectPartsToSelf()
        -> void;

private:

    Model const * model;

    std::string name;

    PartContainer parts;

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

} // namespace ape
