#pragma once

#include <memory>

namespace rave
{

class Application
{

public:

    Application();

    explicit Application(bool enableDebugOutput);

    Application(Application const & rhs) = delete;

    Application(Application && rhs) noexcept;

    auto operator = (Application const & rhs)
        -> Application & = delete;

    auto operator = (Application && rhs) noexcept
        -> Application &;

    ~Application();

    auto run()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace rave
