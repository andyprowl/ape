#pragma once

#include <Ape/InputHandler.hpp>
#include <Ape/ScopedSignalConnection.hpp>

namespace ape
{

class StandardShaderProgram;
class Window;

class BlinnPhongKeySwitcher
{

public:

    BlinnPhongKeySwitcher(Window const & window, StandardShaderProgram & shader);

    auto toggleBlinnPhongModel() const
        -> void;

private:

    auto registerKeyPressedHandler(Window const & window) const
        -> ScopedSignalConnection;

private:

    StandardShaderProgram * shader;

    ScopedSignalConnection onKeyPressedConnection;

};

} // namespace ape
