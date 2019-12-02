#pragma once

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

#include <memory>
#include <string_view>

namespace ape
{

class Ape;

class GlfwGateway
{

public:

    class CreateAsFullscreen
    {

    public:

        explicit CreateAsFullscreen() = default;

    };

public:

    GlfwGateway(int majorVersion, int minorVersion, bool enableDebugOutput);

    GlfwGateway(GlfwGateway && rhs) noexcept;

    auto operator = (GlfwGateway && rhs) noexcept
        -> GlfwGateway &;

    ~GlfwGateway();

    auto createWindow(std::string_view title, CreateAsFullscreen)
        -> GlfwWindow;

    auto createWindow(std::string_view title, basix::Size<int> const & size)
        -> GlfwWindow;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
