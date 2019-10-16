#include <Ape/Scene/BodySelector.hpp>

#include <Ape/Scene/Scene.hpp>

#include <Foundational/Range/Removal.hpp>
#include <Foundational/Range/Transform.hpp>

#include <algorithm>
#include <optional>

namespace ape
{

namespace
{

auto getAddressOfFirstBody(Scene & scene)
    -> Body const *
{
    auto const & bodies = scene.getBodies();

    if (bodies.empty())
    {
        return nullptr;
    }

    return &bodies.front();
}

auto asMutableBodyFromScene(Body const & body, Scene & scene)
    -> Body &
{
    auto const & bodies = scene.getBodies();

    if (bodies.empty())
    {
        throw BodyNotInScene{};
    }

    if ((&body < &bodies.front()) || (&body > &bodies.back()))
    {
        throw BodyNotInScene{};
    }

    auto const index = static_cast<int>(std::distance(&bodies.front(), &body));

    return scene.getBody(index);
}

auto getAllMutableBodiesFromScene(Scene & scene)
    -> std::vector<Body *>
{
    auto const & sceneBodies = scene.getBodies();

    return transform(sceneBodies, [&scene] (Body const & body)
    {
        return &asMutableBodyFromScene(body, scene);
    });
}

} // unnamed namespace

BodySelector::BodySelector(Scene & scene)
    : scene{&scene}
    , firstBodyInScene{getAddressOfFirstBody(scene)}
    , nonSelectedBodies{getAllMutableBodiesFromScene(scene)}
    , bodyReallocationHandlerConnection{registerBodyReallocationHandler()}
    , bodyAddedHandlerConnection{registerBodyAddedHandler()}
{
}

auto BodySelector::getScene() const
    -> Scene &
{
    return *scene;
}

auto BodySelector::getSelectedBodies() const
    -> BodyRange
{
    return BodyRange{selectedBodies};
}

auto BodySelector::getNonSelectedBodies() const
    -> BodyRange
{
    return BodyRange{nonSelectedBodies};
}

auto BodySelector::isBodySelected(Body const & body) const
    -> bool
{
    auto const it = std::find(std::cbegin(selectedBodies), std::cend(selectedBodies), &body);

    return (it != std::cend(selectedBodies));
}

auto BodySelector::selectBody(Body const & body)
    -> void
{
    auto & mutableBody = asMutableBodyFromScene(body, *scene);

    if (isBodySelected(body))
    {
        return;
    }

    selectedBodies.push_back(&mutableBody);

    remove(nonSelectedBodies, &mutableBody);

    onBodySelected.fire(mutableBody);

    onSelectionChanged.fire();
}

auto BodySelector::deselectBody(Body const & body)
    -> void
{
    auto & mutableBody = asMutableBodyFromScene(body, *scene);

    if (!isBodySelected(body))
    {
        return;
    }

    nonSelectedBodies.push_back(&mutableBody);

    remove(selectedBodies, &mutableBody);

    onBodyDeselected.fire(mutableBody);

    onSelectionChanged.fire();
}

auto BodySelector::deselectAllBodies()
    -> void
{
    auto deselectedBodies = selectedBodies;

    nonSelectedBodies.insert(
        std::end(nonSelectedBodies),
        std::cbegin(selectedBodies),
        std::cend(selectedBodies));

    selectedBodies.clear();

    onSelectionChanged.fire();

    for (auto const deselectedBody : deselectedBodies)
    {
        onBodyDeselected.fire(*deselectedBody);
    }
}

auto BodySelector::registerBodyReallocationHandler()
    -> ScopedSignalConnection
{
    return scene->onBodyReallocation.registerHandler([this]
    {
        restoreValidBodyReferences(selectedBodies);

        restoreValidBodyReferences(nonSelectedBodies);

        firstBodyInScene = getAddressOfFirstBody(*scene);
    });
}

auto BodySelector::registerBodyAddedHandler()
    -> ScopedSignalConnection
{
    return scene->onBodyAdded.registerHandler([this] (Body & body)
    {
        nonSelectedBodies.push_back(&body);
    });
}

auto BodySelector::restoreValidBodyReferences(std::vector<Body *> & bodies) const
    -> void
{
    std::transform(
        std::begin(bodies),
        std::end(bodies),
        std::begin(bodies),
        [this] (Body const * const body)
    {
        auto const index = getBodyIndexInOriginalContainer(*body);

        return &(scene->getBody(index));
    });
}

auto BodySelector::getBodyIndexInOriginalContainer(Body const & body) const
    -> int
{
    return static_cast<int>(std::distance(firstBodyInScene, &body));
}

} // namespace ape
