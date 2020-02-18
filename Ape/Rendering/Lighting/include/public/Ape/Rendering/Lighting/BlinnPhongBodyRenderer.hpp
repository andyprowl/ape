#pragma once

#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class BlinnPhongShaderProgram;
class Body;
class BodyPart;
class BodyPartMesh;
class Camera;
class Culler;
class Fog;
class LightSystemUniformSetter;
class LightSystemViewUniformSetter;
class MaterialSetUniformSetter;
class Mesh;
class ShadowMapping;
class ShapeDrawer;

class BlinnPhongBodyRenderer
{

public:

    BlinnPhongBodyRenderer(
        BlinnPhongShaderProgram & shader,
        LightSystemUniformSetter & lightSystemSetter,
        LightSystemViewUniformSetter & lightSystemViewSetter,
        MaterialSetUniformSetter & materialSetSetter,
        ShapeDrawer & shapeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        Fog const & fog,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

private:

    auto setupInvariantUniforms(
        Camera const & camera,
        Fog const & fog,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto getVisibleMeshesSortedByDistanceFromCamera(
        BodyRange const & bodies,
        Camera const & camera) const
        -> std::vector<const BodyPartMesh *>;

    auto getVisibleMeshes(BodyRange const & bodies, Camera const & camera) const
        -> std::vector<const BodyPartMesh *>;

    auto setupBodyPartUniforms(
        BodyPart const & part,
        glm::mat4 const & cameraTransformation) const
        -> void;

    auto isVisible(BodyPartMesh const & mesh, Culler const & culler) const
        -> bool;

    auto renderMesh(BodyPartMesh const & mesh) const
        -> void;

private:

    BlinnPhongShaderProgram * shader;

    LightSystemUniformSetter * lightSystemSetter;

    LightSystemViewUniformSetter * lightSystemViewSetter;

    MaterialSetUniformSetter * materialSetSetter;

    ShapeDrawer * shapeRenderer;

    bool performFrustumCulling;

};

} // namespace ape
