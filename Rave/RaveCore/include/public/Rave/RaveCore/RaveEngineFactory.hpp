#pragma once

#include <Rave/RaveCore/RaveInputHandler.hpp>

#include <Ape/Engine/Engine/Engine.hpp>
#include <Ape/Engine/Windowing/Window.hpp>
#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>

#include <type_traits>

namespace rave
{

class ApplicationObjects;

class RaveEngineFactory
{

public:

    explicit RaveEngineFactory(bool const excludeSponza);

    RaveEngineFactory(RaveEngineFactory && rhs) noexcept;

    auto operator = (RaveEngineFactory && rhs) noexcept
        -> RaveEngineFactory &;

    ~RaveEngineFactory();

    auto getScene()
        -> ape::Scene &;

    template<
        typename EngineType,
        typename WindowType,
        std::enable_if_t<
            std::is_base_of<ape::Engine, EngineType>{} &&
            std::is_base_of<ape::Window, WindowType>{}> * = nullptr>
    auto makeEngine(WindowType & window)
        -> std::unique_ptr<EngineType>
    {
        // TODO: Consider replacing contorted ownership patterns like this one with shared_ptr
        // ownership.

        class OwningEngine : public EngineType
        {

        public:

            OwningEngine(
                WindowType & window,
                std::unique_ptr<ape::Renderer> renderer,
                std::unique_ptr<RaveInputHandler> inputHandler,
                std::unique_ptr<ape::CameraSelector> selector)
                : EngineType{window, std::move(renderer), std::move(inputHandler)}
                , selector{std::move(selector)}
            {
            }

        private:

            std::unique_ptr<ape::CameraSelector> selector;

        };

        auto cameraSelector = std::make_unique<ape::CameraSelector>(getScene());

        auto renderer = makeSceneRenderer(window, *cameraSelector);
        
        auto handler = makeInputHandler(*renderer, *cameraSelector);

        return std::make_unique<OwningEngine>(
            window,
            std::move(renderer),
            std::move(handler),
            std::move(cameraSelector));
    }

    template<
        typename EngineType,
        typename WindowType,
        std::enable_if_t<
            std::is_base_of<ape::Engine, EngineType>{} &&
            std::is_base_of<ape::Window, WindowType>{}> * = nullptr>
    auto makeEngine(WindowType & window, ape::CameraSelector & cameraSelector)
        -> std::unique_ptr<EngineType>
    {
        auto renderer = makeSceneRenderer(window, cameraSelector);
        
        auto handler = makeInputHandler(*renderer, cameraSelector);

        return std::make_unique<EngineType>(window, std::move(renderer), std::move(handler));
    }

private:

    class EngineObjects;

private:

    auto makeSceneRenderer(ape::Window & window, ape::CameraSelector & cameraSelector)
        -> std::unique_ptr<ape::SceneRenderer>;

    auto makeInputHandler(
        ape::SceneRenderer & sceneRenderer,
        ape::CameraSelector & cameraSelector)
        -> std::unique_ptr<RaveInputHandler>;

private:

    std::unique_ptr<EngineObjects> engineObjects;

};

} // namespace rave
