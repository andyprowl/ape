#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>

#include "Glfw.hpp"
#include "GlfwImGuiBinding.hpp"
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
        , lastFrameProfiles{60 * 30} // 30 seconds worth of frame profiles when running at 60 FPS
        , resizeHandlerConnection{registerWindowResizeHandler()}
        , stopBeforeNextIteration{false}
    {
        auto const size = window.getSize();

        setViewport(size);

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        [[maybe_unused]]
        auto & io = ImGui::GetIO();

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window.getGlfwHandle(), true);

        ImGui_ImplOpenGL3_Init("#version 450");
    }

    auto start()
        -> void
    {
        while (!shouldStop())
        {
            processOneFrame();

            recordFrameProfile();

            displayInspectionOverlays();
        }
    }

    auto stop()
        -> void
    {
        stopBeforeNextIteration = true;
    }

private:
    
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

        window->swapBuffers();
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
        auto & profile = profiler.getRootTaskProfile();

        lastFrameProfiles.push_back(std::move(profile));
    }

    auto displayInspectionOverlays() const
        -> void
    {
        // TODO: display captured frame profiles using ImGui.

        static float f = 0.0f;
        static int counter = 0;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");

        ImGui::Text("This is some useful text.");

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);

        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate);

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window->getGlfwHandle(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:

    basix::Stopwatch stopwatch;
    
    GlfwWindow * window;

    SceneRenderer * renderer;

    InputHandler * inputHandler;

    basix::TimeIntervalTracker timeTracker;

    basix::TaskProfiler profiler;

    basix::CircularBuffer<basix::TaskProfile> lastFrameProfiles;

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
