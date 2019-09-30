 - Player camera lookat direction is now different and non-deterministic when ApePlayer starts
  - Works correctly with ApeCreator
 - Let QtEngine and GLFWEngine own the renderer and its context, and enforce current window when
   constructing them
 - Try and generalize GLFW GLFWEngine-based and Qt's event-based approaches
 - Figure out why Blender models get loaded without textures
 - Add CMake install target also handling correct deployment of dependencies
 - Extract utility functions to separate (general-purpose) library projects
 - Support flat shapes/meshes
 - Support "enterable", room-like shapes with an inner and outer layer with inverted normals
