#pragma once

#include <Engine/Pipeline.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SpotLight;

class SpotLightOrientator : public Receiver<glm::mat4>
{

public:

    explicit SpotLightOrientator(SpotLight & light);

    // virtual (from Receiver<glm::mat4>)
    auto receive(glm::mat4 const & transformation)
        -> void override;

private:

    SpotLight * light;

};

} // namespace ape
