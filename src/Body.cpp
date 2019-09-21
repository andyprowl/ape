#include "Body.hpp"

#include "Model.hpp"

namespace
{

auto computeNumOfSubParts(ModelPart const & part)
    -> int
{
    auto numOfSubParts = 0;

    for (auto const & component : part.getComponents())
    {
        numOfSubParts += 1 + computeNumOfSubParts(component);
    }

    return numOfSubParts;
}

auto computeNumOfParts(Model const & model)
    -> int
{
    return 1 + computeNumOfSubParts(model.getRootPart());
}

auto makeSubPartsInstances(
    BodyPart & instance,
    std::vector<BodyPart> & destination)
    -> void
{
    for (auto const & modelSubPart : instance.getModel().getComponents())
    {
        auto & bodySubPart = destination.emplace_back(modelSubPart, instance);

        makeSubPartsInstances(bodySubPart, destination);
    }
}

auto makePartInstances(Model const & model)
    -> std::vector<BodyPart>
{
    auto parts = std::vector<BodyPart>{};

    parts.reserve(computeNumOfParts(model));

    parts.emplace_back(model.getRootPart());

    auto & root = parts.back();

    makeSubPartsInstances(root, parts);

    return parts;
}

} // unnamed namespace

Body::Body(Model const & model)
    : model{&model}
    , parts{makePartInstances(model)}
{
}

auto Body::getModel() const
    -> Model const &
{
    return *model;
}

auto Body::getPart(int const index)
    -> BodyPart &
{
    return parts[index];
}

auto Body::getPart(int const index) const
    -> BodyPart const &
{
    return parts[index];
}

auto Body::getNumOfParts() const
    -> int
{
    return static_cast<int>(parts.size());
}

auto getRootPart(Body & model)
    -> BodyPart &
{
    return model.getPart(0);
}

auto getRootPart(Body const & model)
    -> BodyPart const &
{
    return model.getPart(0);
}

auto getPosition(Body const & model)
    -> glm::vec3
{
    return getLocalPosition(getRootPart(model));
}

auto setPosition(Body & model, glm::vec3 const & newPosition)
    -> void
{
    setLocalPosition(getRootPart(model), newPosition);
}

auto getTransformation(Body const & model)
    -> glm::mat4 const &
{
    return getRootPart(model).getLocalTransformation();
}

auto setTransformation(Body & model, glm::mat4 const & newTransformation)
    -> void
{
    getRootPart(model).setLocalTransformation(newTransformation);
}

auto scaleUniformly(Body & model, float const factor)
    -> void
{
    getRootPart(model).scaleUniformly(factor);
}
