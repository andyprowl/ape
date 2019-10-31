#pragma once

#include <Ape/QtEngine/QtWindow.hpp>

#include <memory>
#include <string_view>

namespace ape::qt
{

class Ape;

class QtGateway
{

public:

    explicit QtGateway(bool enableDebugOutput);

    QtGateway(QtGateway && rhs) noexcept;

    auto operator = (QtGateway && rhs) noexcept
        -> QtGateway &;

    ~QtGateway();

    auto createWindow()
        -> QtWindow *;

    auto createWindow(QWidget & parent)
        -> QtWindow *;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape::qt
