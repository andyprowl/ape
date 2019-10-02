#include <Core/SceneRenderer.hpp>

#include <Core/BodySelector.hpp>
#include <Core/Camera.hpp>
#include <Core/CameraSelector.hpp>
#include <Core/Material.hpp>
#include <Core/ModelPart.hpp>
#include <Core/WireframeShaderProgram.hpp>
#include <Core/Scene.hpp>
#include <Core/Shape.hpp>
#include <Core/StandardShaderProgram.hpp>


// TODO: Clean this up after cleanup of wireframe/outlining!
#include <Core/Stopwatch.hpp>


#include <glad/glad.h>

#include <algorithm>
#include <cassert>

namespace ape
{

SceneRenderer::SceneRenderer(
    RenderingContext const & renderingContext,
    CameraSelector const & cameraSelector,
    BodySelector const & pickedBodySelector,
    StandardShaderProgram & standardShader,
    WireframeShaderProgram & pickingShader,
    glm::vec3 const & backgroundColor)
    : renderingContext{renderingContext}
    , cameraSelector{&cameraSelector}
    , pickedBodySelector{&pickedBodySelector}
    , standardShader{&standardShader}
    , pickingShader{&pickingShader}
    , backgroundColor{backgroundColor}
{
}

auto SceneRenderer::render() const
    -> void
{
    clear();

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }

    standardShader->use();

    setupLighting();

    setupCamera(*activeCamera);

    drawBodies(*activeCamera);
}

auto SceneRenderer::getCameraSelector() const
    -> CameraSelector const &
{
    return *cameraSelector;
}

auto SceneRenderer::setCameraSelector(CameraSelector const & newSelector)
    -> void
{
    cameraSelector = &newSelector;
}

auto SceneRenderer::clear() const
    -> void
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);

    // Culling is not appropriate for all shapes. This should be done conditionally in the future.
    // However, when appropriate, it will save at least 50% of fragment shader calls.
    glEnable(GL_CULL_FACE);

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto SceneRenderer::setupLighting() const
    -> void
{
    auto const & scene = cameraSelector->getScene();

    standardShader->lighting.set(scene.getLighting());
}

auto SceneRenderer::setupCamera(Camera const & camera) const
    -> void
{
    standardShader->camera.set(camera);
}

auto SceneRenderer::drawBodies(Camera const & camera) const
    -> void
{
    arrayObject.bind();
    
    auto const & nonSelectedBodies = pickedBodySelector->getNonSelectedBodies();

    glStencilMask(0x00);
    
    drawBodiesWithStandardShader(camera, nonSelectedBodies);

    auto const & selectedBodies = pickedBodySelector->getSelectedBodies();

    if (selectedBodies.empty())
    {
        return;
    }
    

    //drawBodiesWithPickingShader(camera, selectedBodies);


    //drawBodiesWithStandardShader(camera, selectedBodies);

    
    glStencilFunc(GL_ALWAYS, 1, 0xff);

    glStencilMask(0xff);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    drawBodiesWithStandardShader(camera, selectedBodies);

    glStencilFunc(GL_NOTEQUAL, 1, 0xff);

    glStencilMask(0x00);
    
    drawBodiesWithPickingShader(camera, selectedBodies);
    
    glStencilMask(0xff);
    

    arrayObject.unbind();
}

auto SceneRenderer::drawBodiesWithStandardShader(
    Camera const & camera,
    std::vector<Body *> const & bodies) const
    -> void
{
    standardShader->use();

    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        drawBodyWithStandardShader(*body, cameraTransformation);
    }
}

auto SceneRenderer::drawBodyWithStandardShader(
    Body const & body,
    glm::mat4 const & transformation) const
    -> void
{
    for (auto const & part : body.getParts())
    {
        drawBodyPartWithStandardShader(part, transformation);
    }
}

auto SceneRenderer::drawBodyPartWithStandardShader(
    BodyPart const & part,
    glm::mat4 const & cameraTransformation) const
    -> void
{
    auto const & modelTransformation = part.getGlobalTransformation();

    standardShader->modelTransformation = modelTransformation;

    standardShader->cameraTransformation = cameraTransformation * modelTransformation;

    standardShader->normalTransformation = part.getGlobalNormalTransformation();

    for (auto const mesh : part.getModel().getMeshes())
    {
        drawMeshWithStandardShader(*mesh);
    }
}

auto SceneRenderer::drawMeshWithStandardShader(Mesh const & mesh) const
    -> void
{
    auto const & material = mesh.getMaterial();

    standardShader->materialAmbient = material.ambient;

    standardShader->materialShininess = material.shininess;

    if (material.diffuseMap)
    {
        material.diffuseMap->bind(0);
    }
    
    if (material.specularMap)
    {
        material.specularMap->bind(1);
    }

    mesh.getShape().draw(renderingContext);
}

auto SceneRenderer::drawBodiesWithPickingShader(
    Camera const & camera,
    std::vector<Body *> const & bodies) const
    -> void
{
    static auto stopwatch = Stopwatch{};

    auto elapsedSeconds = stopwatch.getElapsedTime().count() / 1'000'000'000.0;

    auto pulser = glm::sin(elapsedSeconds * 6.0) / 4.0;

    pickingShader->use();

    pickingShader->borderColor = glm::vec3{0.2f + pulser, 0.2f + pulser, 1.0f};
    
    auto const & cameraTransformation = camera.getTransformation();

    for (auto const body : bodies)
    {
        for (auto const & part : body->getParts())
        {
            auto const & modelTransformation = part.getGlobalTransformation();

            pickingShader->transformation = cameraTransformation * modelTransformation;

            for (auto const mesh : part.getModel().getMeshes())
            {
                mesh->getShape().draw(renderingContext);
            }
        }
    }
}

auto getCamera(SceneRenderer const & renderer)
    -> Camera *
{
    return renderer.getCameraSelector().getActiveCamera();
}

auto getScene(SceneRenderer const & renderer)
    -> Scene &
{
    return renderer.getCameraSelector().getScene();
}

} // namespace ape
