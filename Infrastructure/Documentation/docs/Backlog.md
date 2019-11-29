- Use imgui to draw frame duration stats as an overlay
- Try glPushDebugGroup/glPopDebugGroup to mark rendering steps
- Write tutorial on radar approach to frustum culling for spheres
- Write tutorial/documentation on gamma correction
- Write tutorial/documentation on skyboxes
- Write tutorial about view matrix
- Implement view frustum culling for regular rendering - In particular mention how to handle the case where Zc < 0 (not on LightHouse3d) 
 - Aggregate body part mesh bounding volumes into part/aggregate bounding volumes (BHV) and modify
   culling to first test the aggregate, and if it is entirely inside the frustum, skip testing of
   the sub-objects
- Implement tight culling for shadow mapping
 - Have a culling manager listening to camera changes that recomputes frusta and use the same one
   culling manager everywhere, so we don't have to recompute the camera's frustum for every light
   (or light face)
    - Use that in the perspective culler too for detecting if the camera's frustum and the light
      frustum intersect (light frustum also needs to be recomputed only when the light changes)
 - Do not regenerate shadow maps when light or camera do not move
    - Actually, try rendering all the objects without culling them and regenerate the shadow maps
      only when the lights (not camera) move
        - This won't actually work because we need different resolutions based on camera position
- Implement occlusion culling
- Implement outlining by pushing vertices in the direction of normals
- Is it correct that DepthBodyRenderer sets viewport while other renderers do not?
    - The reason for this is that depth body renderer has to bind framebuffer targets
    - Not sure it is correct that the renderer does the binding and setting of viewport though
    - Maybe it is a naming issue? It simply shouldn't be called "renderer"?
    - Try to think how the generic pipeline would like if composed declaratively by the user
- Glow should be independent of GLM: specializations of Uniform for glm types should be in Ape
- Try FBX format for 3D models
    - Use FBX SDK for importing the models
- The "Inversion" post-processing effect no longer seems to work
    - The image is extremely bright
    - Is this related to gamma?
    - I think it looked OK before we did omnidirectional shadow mapping
- Complete implementation of shadow mapping
   - Tightly fit lightSystem view matrices to the scene
   - Implement cascaded shadow maps at least for directional lights
   - Fix warning about texture base level and binding using NSight diagnostic tool (the standalone
     application, not the VS plugin)
       - This has been worked around by allowing to turn off debug mode from command line
   - Consider implementing (restoring?) PCF
   - Shadows seem to be neverending, this is not realistic
       - Try not using 1.0 vs 0.0 but something in the middle that changes with distance
       - Also, try playing with ambient lightSystem to compensate for this
- Performance has dropped a lot in ApeStudio, at least when not plugged to power
     - Try profiling through NSight
- Shouldn't LightSystemView cameras have the aspect ratio of the depth map rather than the aspect
  ratio of the display window?
    - In particular for point light shadow maps, if vertical FOV is 90 and AR is not 1, we get a
    - HFOV which is greater than (or lower than) 90, which means redundant or insufficient view
    - coverage 
- Qt window resizing has stopped working again
    - In fact it seems even going back to the commit that "fixed" it no longer works
- Implement normal maps
    - Have AssetLoader read normal maps
    - Have shader apply normal maps
- EffectSelector, SkyboxSelector, CameraSelector can be instances of the same template
    - Remove duplication by extracting the template
    - Same redundancy for EffectCollection, SkyboxCollection
- Create depth of field effect
- Unify the way GLFW windows and QT windows are created
    - Ideally, there should be a single "Engine" class
- Let shader programs (or at least effects) have names
    - Use that to select the initial effect in ApePlayer and ApeCreator
- Figure out a good way of reducing duplication in effect shaders
- Support wireframe rendering
- Implement techniques to compose effects into chains, possibly reusing a pool of FBOs
- callers of glViewport should do so through the Window object
- Implement normal visualization through geometry shaders
- (Re-)implement glowing based on view position and normal
- Player camera lookat direction is now different and non-deterministic when ApePlayer starts
    - Works correctly with ApeCreator
- Introduce different namespaces for packages (not just "ape")
- Do some performance benchmarking to compare rendering with VAOs and rendering with VBOs
- QtCreator workflow for setting things up is particularly unwieldy. Make it easier
- Refactor handling of Texture readout - should probably be part of AssetLoader, not GpuResource
    - Readout of texture introduces dependency on stb
- Let QtEngine and GLFWEngine own the renderer and its context, and enforce current window when
  constructing them
- Try and generalize GLFWEngine-based and Qt's event-based approaches
- Add CMake install target also handling correct deployment of dependencies
- Support flat shapes/meshes
