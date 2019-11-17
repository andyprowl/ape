#pragma once

#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Body;
class BodyPart;
class BodyPartMesh;
class Camera;
class FrustumCuller;
class LightSystem;
class Mesh;
class ShadowMapping;
class ShapeDrawer;
class BlinnPhongShaderProgram;

class BlinnPhongBodyRenderer
{

public:

    BlinnPhongBodyRenderer(BlinnPhongShaderProgram & shader, ShapeDrawer const & shapeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

private:

    auto setupInvariantUniforms(
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto renderBody(
        Body const & body,
        glm::mat4 const & cameraTransformation,
        FrustumCuller const & culler) const
        -> void;

    auto renderBodyPart(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation,
        FrustumCuller const & culler) const
        -> void;

    auto setupBodyPartUniforms(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation) const
        -> void;

    auto isVisible(BodyPartMesh const & mesh, FrustumCuller const & culler) const
        -> bool;

    auto renderMesh(BodyPartMesh const & mesh) const
        -> void;

private:

    BlinnPhongShaderProgram * shader;

    ShapeDrawer const * shapeRenderer;

    bool performFrustumCulling;

};

} // namespace ape
