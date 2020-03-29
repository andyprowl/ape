#pragma once

#include <Basix/Signal/Signal.hpp>

#include <stdexcept>
#include <memory>
#include <vector>

namespace ape
{

class BlinnPhongShaderProgram;

class ShaderNotContained : public std::logic_error
{

public:

    ShaderNotContained()
        : std::logic_error{"The specified Blinn-Phong shader is not a member of the collection"}
    {
    }

};

class BlinnPhongShaderCollection
{

public:

    BlinnPhongShaderCollection() = default;

    auto addShader(std::unique_ptr<BlinnPhongShaderProgram> shader)
        -> void;

    auto getNumOfShaders() const
        -> int;

    auto getShader(int i)
        -> BlinnPhongShaderProgram &;

    auto getShaderIndex(BlinnPhongShaderProgram const & shader) const
        -> int;

public:

    basix::Signal<auto (BlinnPhongShaderProgram &) -> void> onShaderAdded;

private:

    // Held by unique pointer to avoid invalidation issues.
    std::vector<std::unique_ptr<BlinnPhongShaderProgram>> shaders;

};

} // namespace ape
