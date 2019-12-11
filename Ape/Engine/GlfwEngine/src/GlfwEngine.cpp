#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>

#include "Glfw.hpp"
#include "GlfwImGuiBinding.hpp"
#include "ImGuiFrame.hpp"
#include "OpenGLImGuiBinding.hpp"

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

#include <Ape/Engine/InspectionOverlay/FrameProfilingOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightSystemOverlay.hpp>
#include <Ape/Engine/UpdateHandling/InputHandler.hpp>

#include <Ape/Rendering/GpuProfiling/TaskTimeProfiler.hpp>
#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Windowing/Window.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>
#include <Basix/Time/Stopwatch.hpp>
#include <Basix/Time/TimeIntervalTracker.hpp>

#include <DearImGui/imgui.h>

namespace ape
{

class GlfwEngine::Impl
{

public:

    Impl(GlfwWindow & window, SceneRenderer & renderer, InputHandler & inputHandler)
        : window{&window}
        , renderer{&renderer}
        , inputHandler{&inputHandler}
        , timeTracker{stopwatch}
        , profiler{}
        , lastFrameProfiles{60 * 10} // 10 seconds worth of frame profiles when running at 60 FPS
        , frameProfilingOverlay{makeFrameProfilingOverlay()}
        , lightSystemOverlay{makeLightSystemOverlay()}
        , resizeHandlerConnection{registerWindowResizeHandler()}
        , stopBeforeNextIteration{false}
    {
        renderer.setProfiler(&profiler);

        auto const size = window.getSize();

        setViewport(size);

        setupImGui();
    }

    auto start()
        -> void
    {
        while (!shouldStop())
        {
            recordLastFrameProfile();
        
            updateProfilingOptions();

            processOneFrame();
        }
    }

    auto stop()
        -> void
    {
        stopBeforeNextIteration = true;
    }

private:

    auto makeFrameProfilingOverlay()
        -> FrameProfilingOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 10};

        auto const initialSize = basix::Size<int>{window->getSize().width - 20, 280};

        return {initialPosition, initialSize, profiler, lastFrameProfiles};
    }

    auto makeLightSystemOverlay() const
        -> LightSystemOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 280};

        auto const initialSize = basix::Size<int>{(window->getSize().width - 20) / 2, 650};

        return {initialPosition, initialSize, getScene(*renderer).getLightSystem()};
    }
    
    auto registerWindowResizeHandler()
        -> basix::ScopedSignalConnection
    {
        return window->onResize.registerHandler([this] (basix::Size<int> const & size)
        {
            setViewport(size);
        });
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

        updateAllCameraAspectRatio();
    }

    auto setupImGui()
        -> void
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui::GetStyle().Alpha = 0.2f;

        auto const fontFilePath = resourceFolder "/fonts/ProggyClean.ttf";

        auto & io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF(fontFilePath, 26);

        ImGui_ImplGlfw_InitForOpenGL(window->getGlfwHandle(), true);

        ImGui_ImplOpenGL3_Init("#version 450");
    }

    auto updateAllCameraAspectRatio()
        -> void
    {
        auto const aspectRatio = window->getAspectRatio();

        auto & scene = getScene(*renderer);

        for (auto & camera : scene.getCameras())
        {
            auto const perspective = camera.getProjection().tryAs<PerspectiveProjection>();

            if (perspective != nullptr)
            {
                perspective->setAspectRatio(aspectRatio);
            }
        }
    }

    auto shouldStop() const
        -> bool
    {
        return (stopBeforeNextIteration || window->isClosing());
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
        if (!profiler.isCpuProfilingEnabled() && !profiler.isGpuProfilingEnabled())
        {
            return;
        }

        auto & profile = profiler.getRootProfiledTask();

        if (profile.getMetrics().empty())
        {
            return;
        }

        lastFrameProfiles.push_back(std::move(profile));
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
        auto const profiling = profiler.startTimingCpuGpuTask("Input handling");

        glfwPollEvents();

        auto const lastFrameDuration = timeTracker.tick();

        inputHandler->onFrame(lastFrameDuration);
    }

    auto render()
        -> void
    {
        if (!isWindowReady())
        {
            return;
        }

        auto const profiling = profiler.startTimingCpuGpuTask("Scene rendering");

        renderer->render();
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
        profiler.fetchGpuTimingResults();
    }

    auto isWindowReady() const
        -> bool
    {
        auto const size = window->getSize();

        return (size.height > 0);
    }

    auto updateInspectionOverlays()
        -> void
    {
        auto const profiling = profiler.startTimingCpuGpuTask("Inspection overlay update");

        auto const newFrame = ImGuiFrame{};

        frameProfilingOverlay.update();

        lightSystemOverlay.update();
    }

private:

    basix::Stopwatch stopwatch;
    
    GlfwWindow * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    basix::TimeIntervalTracker timeTracker;

    TaskTimeProfiler profiler;

    //TaskTimeProfiler backProfiler;

    basix::CircularBuffer<basix::ProfiledTask> lastFrameProfiles;

    FrameProfilingOverlay frameProfilingOverlay;

    LightSystemOverlay lightSystemOverlay;

    basix::ScopedSignalConnection resizeHandlerConnection;

    bool stopBeforeNextIteration;

};

GlfwEngine::GlfwEngine(GlfwWindow & window, SceneRenderer & renderer, InputHandler & inputHandler)
    : impl{std::make_unique<Impl>(window, renderer, inputHandler)}
{
}

GlfwEngine::GlfwEngine(GlfwEngine &&) noexcept = default;

auto GlfwEngine::operator = (GlfwEngine &&) noexcept
    -> GlfwEngine & = default;

GlfwEngine::~GlfwEngine() = default;

auto GlfwEngine::start()
    -> void
{
    return impl->start();
}

auto GlfwEngine::stop()
    -> void
{
    return impl->stop();
}

} // namespace ape
