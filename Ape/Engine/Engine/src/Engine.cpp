#include <Ape/Engine/Engine/Engine.hpp>

#include "OpenGLImGuiBinding.hpp"

#include <Ape/Engine/Engine/EventSystem.hpp>
#include <Ape/Engine/Engine/FrameLoop.hpp>
#include <Ape/Engine/Engine/ImGuiEventDispatcher.hpp>
#include <Ape/Engine/Engine/ImGuiFrame.hpp>
#include <Ape/Engine/Engine/InputHandler.hpp>
#include <Ape/Engine/Engine/Renderer.hpp>

#include <Ape/Engine/FrameProfiling/FrameRateCalculator.hpp>
#include <Ape/Engine/FrameProfiling/TaskTimeProfiler.hpp>
#include <Ape/Engine/InspectionOverlay/FogOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/FrameProfilingOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightSystemOverlay.hpp>
#include <Ape/Engine/Windowing/Viewport.hpp>
#include <Ape/Engine/Windowing/Window.hpp>

#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>
#include <Basix/Time/Stopwatch.hpp>
#include <Basix/Time/TimeIntervalTracker.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

class Engine::Impl
{

public:

    Impl(
        Engine & parent,
        Window & window,
        EventSystem & eventSystem,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler,
        ImGuiEventDispatcher & dispatcher,
        FrameLoop & frameLoop)
        : window{&window}
        , eventSystem{&eventSystem}
        , renderer{std::move(renderer)}
        , inputHandler{std::move(inputHandler)}
        , imGuiDispatcher{&dispatcher}
        , frameLoop{&frameLoop}
        , timeTracker{stopwatch}
        , profiler{}
        , lastFrameProfiles{60 * 10} // 10 seconds worth of frame profiles when running at 60 FPS
        , frameRateCalculator{lastFrameProfiles, 4}
        , frameProfilingOverlay{makeFrameProfilingOverlay()}
        , lightSystemOverlay{makeLightSystemOverlay()}
        , fogOverlay{makeFogOverlay()}
        , resizeHandlerConnection{registerWindowResizeHandler()}
        , activeCameraChangeConnection{registerActiveCameraChangeHandler()}
        , isInspectionOverlayShown{false}
    {
        this->renderer->setProfiler(profiler);

        this->inputHandler->setEngine(parent);

        initializeViewport();

        setupImGui();
    }

    auto run()
        -> void
    {
        frameLoop->start([this]
        {
            recordLastFrameProfile();
        
            updateProfilingOptions();

            processOneFrame();
        });
    }

    auto pause()
        -> void
    {
        frameLoop->stop();
    }

    auto isInspectionOverlayVisible() const
        -> bool
    {
        return isInspectionOverlayShown;
    }

    auto showInspectionOverlay()
        -> void
    {
        isInspectionOverlayShown = true;
    }

    auto hideInspectionOverlay()
        -> void
    {
        isInspectionOverlayShown = false;
    }

    auto getRenderer() const
        -> Renderer &
    {
        return *renderer;
    }

    auto getInputHandler()
        -> InputHandler &
    {
        return *inputHandler;
    }

private:

    auto makeFrameProfilingOverlay()
        -> FrameProfilingOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 10};

        auto const initialSize = basix::Size<int>{window->getSize().width - 20, 280};

        return {initialPosition, initialSize, profiler, lastFrameProfiles, frameRateCalculator};
    }

    auto makeLightSystemOverlay() const
        -> LightSystemOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 280};

        auto const initialSize = basix::Size<int>{(window->getSize().width - 20) / 2, 550};

        auto & scene = renderer->getCameraSelector().getScene();

        return {initialPosition, initialSize, scene.getLightSystem()};
    }

    auto makeFogOverlay() const
        -> FogOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 280 + 550};

        auto const initialSize = basix::Size<int>{(window->getSize().width - 20) / 2, 120};

        auto & scene = renderer->getCameraSelector().getScene();

        return {initialPosition, initialSize, scene.getFog()};
    }
    
    auto registerWindowResizeHandler()
        -> basix::ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (basix::Size<int> const & newSize)
        {
            setViewport(newSize);
        });
    }

    auto registerActiveCameraChangeHandler()
        -> basix::ScopedSignalConnection
    {
        auto const & cameraSelector = renderer->getCameraSelector();

        return cameraSelector.onActiveCameraChanged.registerHandler([this] (Camera const *)
        {
            auto const size = window->getSize();

            updateActiveCameraAspectRatio(size);
        });
    }

    auto initializeViewport()
        -> void
    {
        auto const size = window->getSize();

        setViewport(size);
    }

    auto setViewport(basix::Size<int> const & size)
        -> void
    {
        if (size.height == 0)
        {
            return;
        }

        auto const origin = basix::Position{0, 0};

        renderer->setViewport({origin, size});

        updateActiveCameraAspectRatio(size);
    }

    auto setupImGui()
        -> void
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui::GetStyle().Alpha = 0.2f;

        ImGui_ImplOpenGL3_Init("#version 450");

        imGuiDispatcher->initialize(*window);

        auto & io = ImGui::GetIO();

        auto const fontFilePath = resourceFolder "/fonts/ProggyClean.ttf";

        io.Fonts->AddFontFromFileTTF(fontFilePath,  26);
    }

    auto updateActiveCameraAspectRatio(basix::Size<int> const & size)
        -> void
    {
        auto const camera = renderer->getCameraSelector().getActiveCamera();

        if (camera == nullptr)
        {
            return;
        }

        auto perspective = camera->getProjection().tryAs<PerspectiveProjection>();

        if (perspective == nullptr)
        {
            return;
        }

        auto const aspectRatio = static_cast<float>(size.width) / size.height;

        perspective->setAspectRatio(aspectRatio);
    }

    auto processOneFrame()
        -> void
    {
        auto const profiling = profiler.startTimingCpuGpuTask("Frame processing");

        processInput();

        render();

        updateInspectionOverlays();

        swapBuffers();

        fetchGpuTimeQueryResults();
    }

    auto recordLastFrameProfile()
        -> void
    {
        if (not profiler.isCpuProfilingEnabled() && not profiler.isGpuProfilingEnabled())
        {
            return;
        }

        auto & profile = profiler.getRootProfiledTask();

        if (profile.getMetrics().empty())
        {
            return;
        }

        lastFrameProfiles.push_back(std::move(profile));

        frameRateCalculator.update();
    }

    auto updateProfilingOptions()
        -> void
    {
        if (frameProfilingOverlay.isProfilingPaused())
        {
            profiler.disableProfiling();

            return;
        }

        profiler.enableCpuProfiling();

        if (frameProfilingOverlay.isGpuTimeCollectionEnabled())
        {
            profiler.enableGpuProfiling();
        }
        else
        {
            profiler.disableGpuProfiling();
        }
    }

    auto processInput()
        -> void
    {
        auto const profiling = profiler.startTimingCpuTask("Input handling");

        eventSystem->pollEvents();

        auto const lastFrameDuration = timeTracker.tick();

        inputHandler->onFrame(lastFrameDuration);
    }

    auto render()
        -> void
    {
        if (not isWindowReady())
        {
            return;
        }

        auto const profiling = profiler.startTimingCpuGpuTask("Scene rendering");

        renderer->render();
    }

    auto updateInspectionOverlays()
        -> void
    {
        if (not isInspectionOverlayShown)
        {
            return;
        }

        auto const profiling = profiler.startTimingCpuGpuTask("Inspection overlay update");

        auto const newFrame = ImGuiFrame{*imGuiDispatcher};

        frameProfilingOverlay.update();

        lightSystemOverlay.update();

        fogOverlay.update();
    }

    auto swapBuffers()
        -> void
    {
        auto const profiling = profiler.startTimingCpuGpuTask("Swapping buffers");

        window->swapBuffers();
    }

    auto fetchGpuTimeQueryResults()
        -> void
    {
        auto const profiling = profiler.startTimingCpuGpuTask("Fetching GPU timing results");

        profiler.fetchGpuTimingResults();
    }

    auto isWindowReady() const
        -> bool
    {
        auto const size = window->getSize();

        return (size.height > 0);
    }

private:

    basix::Stopwatch stopwatch;
    
    Window * window;

    EventSystem * eventSystem;

    std::unique_ptr<Renderer> renderer;

    std::unique_ptr<InputHandler> inputHandler;

    ImGuiEventDispatcher * imGuiDispatcher;

    FrameLoop * frameLoop;

    basix::TimeIntervalTracker timeTracker;

    TaskTimeProfiler profiler;

    basix::CircularBuffer<basix::ProfiledTask> lastFrameProfiles;

    FrameRateCalculator frameRateCalculator;

    FrameProfilingOverlay frameProfilingOverlay;

    LightSystemOverlay lightSystemOverlay;

    FogOverlay fogOverlay;

    basix::ScopedSignalConnection resizeHandlerConnection;

    basix::ScopedSignalConnection activeCameraChangeConnection;

    bool isInspectionOverlayShown;

};

Engine::Engine(
    Window & window,
    EventSystem & eventSystem,
    std::unique_ptr<Renderer> renderer,
    std::unique_ptr<InputHandler> inputHandler,
    ImGuiEventDispatcher & dispatcher,
    FrameLoop & frameLoop)
    : impl{std::make_unique<Impl>(
        *this,
        window,
        eventSystem,
        std::move(renderer),
        std::move(inputHandler),
        dispatcher,
        frameLoop)}
{
}

Engine::Engine(Engine &&) noexcept = default;

auto Engine::operator = (Engine &&) noexcept
    -> Engine & = default;

Engine::~Engine() = default;

auto Engine::run()
    -> void
{
    return impl->run();
}

auto Engine::pause()
    -> void
{
    return impl->pause();
}

auto Engine::isInspectionOverlayVisible() const
    -> bool
{
    return impl->isInspectionOverlayVisible();
}

auto Engine::showInspectionOverlay()
    -> void
{
    return impl->showInspectionOverlay();
}

auto Engine::hideInspectionOverlay()
    -> void
{
    return impl->hideInspectionOverlay();
}

auto Engine::getRenderer() const
    -> Renderer &
{
    return impl->getRenderer();
}

auto Engine::getInputHandler()
    -> InputHandler &
{
    return impl->getInputHandler();
}

auto getScene(Engine const & engine)
    -> Scene &
{
    auto & renderer = engine.getRenderer();

    auto & cameraSelector = renderer.getCameraSelector();

    return cameraSelector.getScene();
}

auto getWindow(Engine const & engine)
    -> Window &
{
    auto & renderer = engine.getRenderer();

    return renderer.getTargetSurface();
}

} // namespace ape
