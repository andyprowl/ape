#pragma once

#include "SampleAssetCollection.hpp"
#include "SampleInputHandler.hpp"
#include "SampleScene.hpp"

#include <Ape/Engine.hpp>
#include <Ape/Window.hpp>

#include <memory>

class Application
{

public:

    Application();

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