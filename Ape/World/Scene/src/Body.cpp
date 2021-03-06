#include <Ape/World/Scene/Body.hpp>

#include <Ape/World/Model/Model.hpp>

namespace ape
{

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
    BodyPart & bodyPart,
    Body & body,
    std::vector<BodyPart> & destination)
    -> void
{
    for (auto const & modelSubPart : bodyPart.getModel().getComponents())
    {
        auto & bodySubPart = destination.emplace_back(body, modelSubPart);

        makeSubPartsInstances(bodySubPart, body, destination);
    }
}

auto makePartInstances(Body & body, Model const & model)
    -> std::vector<BodyPart>
{
    auto parts = std::vector<BodyPart>{};

    parts.reserve(computeNumOfParts(model));

    parts.emplace_back(body, model.getRootPart());

    auto & root = parts.back();

    makeSubPartsInstances(root, body, parts);

    return parts;
}

} // unnamed namespace

Body::Body(Model const & model)
    : Body{model, ""}
{
}

Body::Body(Model const & model, std::string name, bool const castsShadow)
    : model{&model}
    , name{std::move(name)}
    , parts{makePartInstances(*this, model)}
    , castsShadow{castsShadow}
{
}

Body::Body(Body && rhs) noexcept
    : model{rhs.model}
    , name{std::move(rhs.name)}
    , parts{std::move(rhs.parts)}
    , castsShadow{rhs.castsShadow}
{
    connectPartsToSelf();
}

auto Body::operator = (Body && rhs) noexcept
    -> Body &
{
    model = rhs.model;

    name = std::move(rhs.name);
    
    parts = std::move(rhs.parts);

    castsShadow = rhs.castsShadow;

    connectPartsToSelf();

    return *this;
}

auto Body::getName() const
    -> std::string_view
{
    return name;
}

auto Body::getModel() const
    -> Model const &
{
    return *model;
}

auto Body::getNumOfParts() const
    -> int
{
    return static_cast<int>(parts.size());
}

auto Body::getPart(int const index)
    -> BodyPart &
{
    return parts[index];
}

auto Body::getPart(int index) const
    -> BodyPart const &
{
    return parts[index];
}

auto Body::getParts()
    -> basix::ContainerView<PartContainer>
{
    return basix::makeView(parts);
}

auto Body::getParts() const
    -> basix::ContainerView<PartContainer const>
{
    return basix::makeView(parts);
}

auto Body::isCastingShadow() const
    -> bool
{
    return castsShadow;
}

auto Body::enableShadowCasting()
    -> void
{
    castsShadow = true;
}

auto Body::disableShadowCasting()
    -> void
{
    castsShadow = false;
}

auto Body::toggleShadowCasting()
    -> void
{
    castsShadow = !castsShadow;
}

auto Body::connectPartsToSelf()
    -> void
{
    for (auto & part : parts)
    {
        part.setBody(*this);
    }
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

} // namespace ape
