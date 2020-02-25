#pragma once

namespace ape
{

class Scene;

class SceneStatisticsCollector
{

public:

    explicit SceneStatisticsCollector(Scene const & scene);

    auto getTotalNumOfTriangles() const
        -> int;

private:

    Scene const * scene;

};

} // namespace ape
