 - Refactor rendering system
  - Let BodyRenderer be stateful and constructed at every call to SceneRenderer::render()
  - Create a TransformingShader that passes the cameraTransformation uniform
  - Make StandardShader and WireframeShader subclasses of TransformingShader
  - Create corresponding subclasses of BodyRenderer that do the setup work with those shaders
  - Is Texture an asset too? It is stored in the asset repository, but it is defined in
    GpuResources...

 - Fix temporary implementation of outlining in SceneRenderer
 - Introduce different namespaces for packages (not just "ape")
 - Do some performance benchmarking to compare rendering with VAOs and rendering with VBOs
 - QtCreator workflow for setting things up is particularly unwieldy. Make it easier
 - Refactor handling of Texture readout - should probably be part of AssetLoader, not GpuResource
  - Readout of texture introduces dependency on stb
 - Player camera lookat direction is now different and non-deterministic when ApePlayer starts
  - Works correctly with ApeCreator
 - Let QtEngine and GLFWEngine own the renderer and its context, and enforce current window when
   constructing them
 - Try and generalize GLFWEngine-based and Qt's event-based approaches
 - Add CMake install target also handling correct deployment of dependencies
 - Support flat shapes/meshes
 - Support "enterable", room-like shapes with an inner and outer layer with inverted normals
