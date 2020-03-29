#pragma once

#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class BlinnPhongShaderProgram;
class BlinnPhongShaderSelector;
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
        BlinnPhongShaderSelector & shaderSelector,
        LightSystemUniformSetter & lightSystemSetter,
        LightSystemViewUniformSetter & lightSystemViewSetter,
        MaterialSetUniformSetter & materialSetSetter,
        ShadowMapping const & shadowMapping,
        ShapeDrawer & shapeDrawer);

    auto render(BodyRange const & bodies, Camera const & camera, Fog const & fog) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

private:

    auto setupInvariantUniforms(
        BlinnPhongShaderProgram & shader,
        Camera const & camera,
        Fog const & fog) const
        -> void;

    auto getVisibleMeshesSortedByDistanceFromCamera(
        BodyRange const & bodies,
        Camera const & camera) const
        -> std::vector<const BodyPartMesh *>;

    auto getVisibleMeshes(BodyRange const & bodies, Camera const & camera) const
        -> std::vector<const BodyPartMesh *>;

    auto isVisible(BodyPartMesh const & mesh, Culler const & culler) const
        -> bool;

private:

    BlinnPhongShaderSelector * shaderSelector;

    LightSystemUniformSetter * lightSystemSetter;

    LightSystemViewUniformSetter * lightSystemViewSetter;

    MaterialSetUniformSetter * materialSetSetter;

    ShadowMapping const * shadowMapping;

    ShapeDrawer * shapeDrawer;

    bool performFrustumCulling;

};

} // namespace ape
