#pragma once

#include "Normal.hpp"
#include "Scene.hpp"
#include "TextureRepository.hpp"

#include <vector>

class Window;

class SceneBuilder
{

public:

    explicit SceneBuilder(Window const & window);

    auto build() const
        -> Scene;

private:

    auto createBodies() const
        -> std::vector<Mesh>;

    auto createGroundTileBodies(std::vector<Mesh> & meshes) const
        -> void;

    auto createGroundTileMesh(
        int const row,
        int const col,
        std::shared_ptr<Shape> shape,
        Material const & material,
        std::vector<Mesh> & meshes) const
        -> void;

    auto getGroundMaterial() const
        -> Material;

    auto createCubeBodies(std::vector<Mesh> & meshes) const
        -> void;

    auto getCubeMeshPositions() const
        -> std::vector<glm::vec3>;

    auto getCubeMeshMaterials() const
        -> std::vector<Material>;

    auto getContainerMaterial() const
        -> Material;

    auto createLampBodies(std::vector<Mesh> & meshes) const
        -> void;

    auto getLampMeshPositions() const
        -> std::vector<glm::vec3>;

    auto getLampMaterial() const
        -> Material;

    auto createFlashLightBodies(std::vector<Mesh> & meshes) const
        -> void;

    auto getFlashLightMeshPositions() const
        -> std::vector<glm::vec3>;

    auto computeFlashLightMeshRotationMatrix(glm::vec3 const & position) const
        -> glm::mat4;
    
    auto getFlashLightMaterial() const
        -> Material;

    auto createLighting() const
        -> Lighting;

    auto createPointLights() const
        -> std::vector<PointLight>;

    auto createPointLight(glm::vec3 const & position, std::vector<PointLight> & lights) const
        -> void;

    auto createSpotLights() const
        -> std::vector<SpotLight>;

    auto getSpotLightColors() const
        -> std::vector<Light::Color>;

    auto createSpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
        Light::Color const & color,
        std::vector<SpotLight> & lights) const
        -> void;

    auto createDirectionalLights() const
        -> std::vector<DirectionalLight>;

    auto getDirectionalLightDirections() const
        -> std::vector<glm::vec3>;

    auto createDirectionalLight(
        glm::vec3 const & direction,
        std::vector<DirectionalLight> & lights) const
        -> void;
    
    auto createCamera() const
        -> Camera;

    auto loadTexture(std::string filename) const
        -> Texture const &;

    auto makeBox(NormalDirection const normalDirection, glm::vec3 const & size) const
        -> std::shared_ptr<Shape>;

private:

    Window const * window;

    mutable TextureRepository textures;

};
