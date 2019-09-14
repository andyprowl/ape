#pragma once

#include "Scene.hpp"
#include "TextureRepository.hpp"

#include <vector>

class ShaderProgram;

struct GLFWwindow;

class SceneBuilder
{

public:

    SceneBuilder(GLFWwindow & window, ShaderProgram const & shader);

    auto build() const
        -> Scene;

private:

    auto createBodies() const
        -> std::vector<Body>;

    auto createGroundTileBodies(std::vector<Body> & bodies) const
        -> void;

    auto createGroundTileBody(
        int const row,
        int const col,
        std::shared_ptr<Shape> shape,
        Material const & material,
        std::vector<Body> & bodies) const
        -> void;

    auto getGroundMaterial() const
        -> Material;

    auto createCubeBodies(std::vector<Body> & bodies) const
        -> void;

    auto getCubeBodyPositions() const
        -> std::vector<glm::vec3>;

    auto getCubeBodyMaterials() const
        -> std::vector<Material>;

    auto getContainerMaterial() const
        -> Material;

    auto createLampBodies(std::vector<Body> & bodies) const
        -> void;

    auto getLampBodyPositions() const
        -> std::vector<glm::vec3>;

    auto getLampMaterial() const
        -> Material;

    auto createFlashLightBodies(std::vector<Body> & bodies) const
        -> void;

    auto getFlashLightBodyPositions() const
        -> std::vector<glm::vec3>;

    auto computeFlashLightBodyRotationMatrix(glm::vec3 const & position) const
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

    auto createSpotLight(
        glm::vec3 const & position,
        glm::vec3 const & direction,
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

private:

    ShaderProgram const * shader;

    GLFWwindow * window;

    mutable TextureRepository textures;

};
