#include <UpdateHandling/SpotLightOrientator.hpp>

#include <Scene/SpotLight.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

SpotLightOrientator::SpotLightOrientator(SpotLight & light)
    : light{&light}
{
}

// virtual (from Receiver<glm::mat4>)
auto SpotLightOrientator::receive(glm::mat4 const & transformation)
    -> void
{
    auto const inv = glm::inverse(transformation);

    auto const newPosition = glm::vec3{inv[3]};

    auto const newDirection = -glm::vec3{inv[2]};

    light->setPlacement(newPosition, newDirection);
}

} // namespace ape
