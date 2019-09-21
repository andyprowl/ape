```
cmake <src-dir> -G "Visual Studio 15 2017 Win64" [-Dassimp_DIR=<cmake-config-dir>] -Dglfw3_DIR=<cmake-config-dir> -Dglm_DIR=<src-dir> -Dstb_DIR=<src-dir> -Dglad_DIR=<src-dir>
```

Example:

```
cmake ../OpenGLTest -G "Visual Studio 15 2017 Win64" -Dassimp_DIR="c:/Program Files/Assimp/lib/cmake/assimp-4.1" -Dglfw3_DIR="c:/Program Files/GLFW/lib/cmake/glfw3" -Dglm_DIR="c:/Users/H216918/Documents/Visual Studio 2017/Projects/glm-0.9.9.6/glm" -Dstb_DIR="c:/Users/H216918/Documents/Visual Studio 2017/Projects/stb" -Dglad_DIR="c:/Users/H216918/Documents/Visual Studio 2017/Projects/glad"
```

Note: Assimp's explicit directory specification seems to be unnecessary.
