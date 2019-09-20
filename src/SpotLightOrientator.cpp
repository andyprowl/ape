#include "SpotLightOrientator.hpp"

#include "SpotLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

SpotLightOrientator::SpotLightOrientator(SpotLight & light)
    : light{&light}
{
}

// virtual (from Receiver<glm::mat4>)
auto SpotLightOrientator::receive(glm::mat4 const & transformation)
    -> void
{
    auto const inv = glm::inverse(transformation);

    light->position = glm::vec3{inv[3]};

    light->direction = -glm::vec3{inv[2]};
}
