#pragma once

#include <Ape/Scene/BodyRange.hpp>

#include <Basix/Signal/Signal.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace ape
{

class Body;
class Scene;

class BodyNotInScene : public std::logic_error
{

public:

    BodyNotInScene()
        : logic_error{"The specified body does not belong to the attached scene"}
    {
    }

};

class BodySelector
{

public:

    explicit BodySelector(Scene & scene);

    auto getScene() const
        -> Scene &;

    auto getSelectedBodies() const
        -> BodyRange;

    auto getNonSelectedBodies() const
        -> BodyRange;

    auto isBodySelected(Body const & body) const
        -> bool;

    auto selectBody(Body const & body)
        -> void;

    auto deselectBody(Body const & body)
        -> void;

    auto deselectAllBodies()
        -> void;

public:

    mutable basix::Signal<auto (Body &) -> void> onBodySelected;

    mutable basix::Signal<auto (Body &) -> void> onBodyDeselected;

    mutable basix::Signal<auto () -> void> onSelectionChanged;

private:

    auto registerBodyReallocationHandler()
        -> basix::ScopedSignalConnection;

    auto registerBodyAddedHandler()
        -> basix::ScopedSignalConnection;

    auto restoreValidBodyReferences(std::vector<Body *> & bodies) const
        -> void;

    auto getBodyIndexInOriginalContainer(Body const & body) const
        -> int;

private:

    Scene * scene;

    Body const * firstBodyInScene;

    std::vector<Body *> selectedBodies;

    std::vector<Body *> nonSelectedBodies;

    basix::ScopedSignalConnection bodyReallocationHandlerConnection;

    basix::ScopedSignalConnection bodyAddedHandlerConnection;

};

} // namespace ape
