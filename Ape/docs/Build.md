Example:

```
cmake ..\OpenGLTest -G "Visual Studio 16 2019" -A x64 -Dassimp_DIR="C:\\Code\\Libraries\\Assimp\\x64\\lib\\cmake\\assimp-5.0" -Dglfw3_DIR="C:\\Code\\Libraries\\glfw\\x64\\lib\\cmake\\glfw3" -Dglm_DIR="C:\\Code\\Libraries\\glm-0.9.9.6\\glm" -Wno-dev
```

Note: Assimp's explicit directory specification seems to be unnecessary.
