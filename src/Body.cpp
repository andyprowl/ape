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
    BodyPart const & instance,
    std::vector<BodyPart> & destination)
    -> void
{
    for (auto const & component : instance.getModel().getComponents())
    {
        destination.emplace_back(component, instance);

        auto & componentInstance = destination.back();

        makeSubPartsInstances(componentInstance, destination);
    }
}

auto makePartInstances(Model const & model)
    -> std::vector<BodyPart>
{
    auto instances = std::vector<BodyPart>{};

    instances.reserve(computeNumOfParts(model));

    instances.emplace_back(model.getRootPart());

    auto const & root = instances.back();

    makeSubPartsInstances(root, instances);

    return instances;
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
