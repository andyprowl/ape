#include "ModelInstance.hpp"

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
    ModelPartInstance const & instance,
    std::vector<ModelPartInstance> & destination)
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
    -> std::vector<ModelPartInstance>
{
    auto instances = std::vector<ModelPartInstance>{};

    instances.reserve(computeNumOfParts(model));

    instances.emplace_back(model.getRootPart());

    auto const & root = instances.back();

    makeSubPartsInstances(root, instances);

    return instances;
}

} // unnamed namespace

ModelInstance::ModelInstance(Model const & model)
    : model{&model}
    , parts{makePartInstances(model)}
{
}

auto ModelInstance::getModel() const
    -> Model const &
{
    return *model;
}

auto ModelInstance::getPart(int const index)
    -> ModelPartInstance &
{
    return parts[index];
}

auto ModelInstance::getPart(int const index) const
    -> ModelPartInstance const &
{
    return parts[index];
}

auto ModelInstance::getNumOfParts() const
    -> int
{
    return static_cast<int>(parts.size());
}

auto getRootPart(ModelInstance & model)
    -> ModelPartInstance &
{
    return model.getPart(0);
}

auto getRootPart(ModelInstance const & model)
    -> ModelPartInstance const &
{
    return model.getPart(0);
}

auto getPosition(ModelInstance const & model)
    -> glm::vec3
{
    return getLocalPosition(getRootPart(model));
}

auto setPosition(ModelInstance & model, glm::vec3 const & newPosition)
    -> void
{
    setLocalPosition(getRootPart(model), newPosition);
}

auto getTransformation(ModelInstance const & model)
    -> glm::mat4 const &
{
    return getRootPart(model).getLocalTransformation();
}

auto setTransformation(ModelInstance & model, glm::mat4 const & newTransformation)
    -> void
{
    getRootPart(model).setLocalTransformation(newTransformation);
}

auto scaleUniformly(ModelInstance & model, float const factor)
    -> void
{
    getRootPart(model).scaleUniformly(factor);
}
