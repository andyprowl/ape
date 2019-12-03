#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>

#include "FrameProfilingOverlay.hpp"
#include "Glfw.hpp"
#include "GlfwImGuiBinding.hpp"
#include "ImGuiFrame.hpp"
#include "OpenGLImGuiBinding.hpp"

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

#include <Ape/Engine/UpdateHandling/InputHandler.hpp>

#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Windowing/Window.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Container/CircularBuffer.hpp>
#include <Basix/Profiling/TaskProfiler.hpp>
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
        , lastFrameProfiles{60 * 10} // 10 seconds worth of frame profiles when running at 60 FPS
        , frameProfilingOverlay{makeFrameProfilingOverlay()}
        , resizeHandlerConnection{registerWindowResizeHandler()}
        , doNotRecordFrameProfiles{false}
        , stopBeforeNextIteration{false}
    {
        auto const size = window.getSize();

        setViewport(size);

        setupImGui();
    }

    auto start()
        -> void
    {
        while (!shouldStop())
        {
            processOneFrame();

            recordFrameProfile();

            updateInspectionOverlays();

            window->swapBuffers();
        }
    }

    auto stop()
        -> void
    {
        stopBeforeNextIteration = true;
    }

private:

    auto makeFrameProfilingOverlay() const
        -> FrameProfilingOverlay
    {
        auto const initialPosition =  basix::Position<int>{10, 10};

        auto const initialSize = basix::Size<int>{window->getSize().width - 20, 300};

        return {initialPosition, initialSize, lastFrameProfiles};
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

        [[maybe_unused]]
        auto & io = ImGui::GetIO();

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        ImGui::GetStyle().Alpha = 0.2f;

        auto const fontFilePath = R"(C:\Code\Projects\imgui\source\misc\fonts\ProggyClean.ttf)";

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
        auto const profiling = profiler.startProfilingTask("Frame processing");

        processInput();

        render();
    }

    auto processInput()
        -> void
    {
        auto const profiling = profiler.startProfilingTask("Input handling");

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

        auto const profiling = profiler.startProfilingTask("Scene rendering");

        renderer->render();
    }

    auto isWindowReady() const
        -> bool
    {
        auto const size = window->getSize();

        return (size.height > 0);
    }

    auto recordFrameProfile()
        -> void
    {
        if (doNotRecordFrameProfiles)
        {
            return;
        }

        auto & profile = profiler.getRootTaskProfile();

        lastFrameProfiles.push_back(std::move(profile));
    }

    auto updateInspectionOverlays()
        -> void
    {
        auto const newFrame = ImGuiFrame{};

        frameProfilingOverlay.update();

        doNotRecordFrameProfiles = frameProfilingOverlay.isFrameProfilingPaused();
    }

private:

    basix::Stopwatch stopwatch;
    
    GlfwWindow * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    basix::TimeIntervalTracker timeTracker;

    basix::TaskProfiler profiler;

    basix::CircularBuffer<basix::TaskProfile> lastFrameProfiles;

    FrameProfilingOverlay frameProfilingOverlay;

    basix::ScopedSignalConnection resizeHandlerConnection;

    bool doNotRecordFrameProfiles;

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
