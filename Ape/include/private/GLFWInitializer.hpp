#pragma once

class GLFWInitializer
{

public:

    GLFWInitializer();

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
