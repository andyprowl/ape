#pragma once

namespace ape
{

class GLFWInitializer
{

public:

    GLFWInitializer(int majorVersion, int minorVersion);

    GLFWInitializer(GLFWInitializer const & rhs) = delete;

    GLFWInitializer(GLFWInitializer && rhs) noexcept;

    auto operator = (GLFWInitializer const & rhs)
        -> GLFWInitializer && = delete;

    auto operator = (GLFWInitializer && rhs) noexcept
        -> GLFWInitializer &;

    ~GLFWInitializer();

public:

    bool isInitialized;

};

} // namespace ape
