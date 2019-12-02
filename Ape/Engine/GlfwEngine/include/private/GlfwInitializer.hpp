#pragma once

namespace ape
{

class GlfwInitializer
{

public:

    GlfwInitializer(int majorVersion, int minorVersion);

    GlfwInitializer(GlfwInitializer const & rhs) = delete;

    GlfwInitializer(GlfwInitializer && rhs) noexcept;

    auto operator = (GlfwInitializer const & rhs)
        -> GlfwInitializer && = delete;

    auto operator = (GlfwInitializer && rhs) noexcept
        -> GlfwInitializer &;

    ~GlfwInitializer();

public:

    bool isInitialized;

};

} // namespace ape
