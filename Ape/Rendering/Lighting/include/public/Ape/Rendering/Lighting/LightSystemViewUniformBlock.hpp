#pragma once

#include <Ape/Rendering/Lighting/LightSystemView.hpp>

#include <Glow/Shader/ShaderProgram.hpp>
#include <Glow/Shader/UniformBlock.hpp>
#include <Glow/Shader/UniformBlockMember.hpp>
#include <Glow/Shader/UniformArrayBlockMember.hpp>

namespace glow
{

template<>
class UniformBlockMember<ape::LightSystemView>
{

public:

    using ValueType = ape::LightSystemView;

public:

    UniformBlockMember(UniformBlock & block, std::string const & prefix)
        : spot{block, prefix + ".spot", 8}
        , directional{block, prefix + ".directional", 4}
    {
    }

    auto set(ValueType const & lightSystem, std::byte * const buffer)
        -> void
    {
        auto const & spotView = lightSystem.getSpotView();

        for (auto i = 0; i < static_cast<int>(spotView.size()); ++i)
        {
            auto const & matrix = spotView[i].getCamera().getTransformation();

            // Renormalizes clip space coordinates. See the comment below for normalizationMatrix.
            spot.set(normalizationMatrix * matrix, i, buffer);
        }

        auto const & directionalView = lightSystem.getDirectionalView();

        for (auto i = 0; i < static_cast<int>(directionalView.size()); ++i)
        {
            auto const & matrix = directionalView[i].getCamera().getTransformation();

            // Renormalizes clip space coordinates. See the comment below for normalizationMatrix.
            directional.set(normalizationMatrix * matrix, i, buffer);
        }
    }

public:

    UniformArrayBlockMember<glm::mat4> spot;

    UniformArrayBlockMember<glm::mat4> directional;

private:

    /**
        * Renormalizes clip space coordinates in the [-w, +w] range produced by light camera
        * projection matrices to the [0, +w] range. This way we do not need to perform the same
        * normalization in the fragment shader in order to fetch texels from shadow maps.
    **/
    static constexpr auto const normalizationMatrix = glm::mat4{
        {0.5f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.5f, 0.0f},
        {0.5f, 0.5f, 0.5f, 1.0f}};

};

} // namespace glow

namespace ape
{

class LightSystemViewUniformBlock : public glow::UniformBlock
{

public:

    using ValueType = ape::DirectionalLight;

public:

    LightSystemViewUniformBlock(glow::ShaderProgram & shader, std::string_view const name)
        : UniformBlock{shader, name}
        , lightSystemView{*this, "lightSystemView"}
    {
    }

    auto set(LightSystemView const & view, std::byte * const buffer)
    {
        lightSystemView.set(view, buffer);
    }

public:

    glow::UniformBlockMember<ape::LightSystemView> lightSystemView;

};

} // namespace ape
