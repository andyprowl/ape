#include <Ape/Rendering/Culling/PerspectiveLightCuller.hpp>

#include "CameraFrustum.hpp"

#include <Ape/Rendering/Culling/Frustum.hpp>

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

#include <Basix/Range/Search.hpp>

#include <glm/gtc/matrix_access.hpp>

namespace ape
{

namespace
{

auto makeFrustumPlane(glm::vec3 const & p1, glm::vec3 const & p2, glm::vec3 const & p3)
    -> Plane
{
    const auto normal = glm::normalize(glm::cross((p2 - p1), (p3 - p1)));
    
    const auto offset = glm::dot(-p1, normal);

    return {normal, offset};
}

class BoundingVolumeCalculator
{

public:

    BoundingVolumeCalculator(
        Camera const & lightPerspective,
        Camera const & viewerPerspective)
        : lightPosition{lightPerspective.getView().getPosition()}
        , lightFrustum{extractCameraFrustum(lightPerspective)}
        , viewerFrustum{extractCameraFrustum(viewerPerspective)}
    {
    }

    auto calculate() const
    {
        auto volume = ConvexVolume{};

        addViewerPerspectiveBounds(volume);

        addLightPerspectiveBounds(volume);

        return volume;
    }

private:

    auto addViewerPerspectiveBounds(ConvexVolume & volume) const
        -> void
    {
        for (auto const face : getAllFaces())
        {
            for (auto const neighborFace : getNeighborFaces(face))
            {
                addIfDelimiterEdge(face, neighborFace, volume);

                if (volume.getPlanes().size() == 6u)
                {
                    return;
                }
            }
        }
    }

    auto addLightPerspectiveBounds(ConvexVolume & volume) const
        -> void
    {
        for (auto const & plane : lightFrustum.getPlanes())
        {
            volume.addPlane(plane);
        }
    }

    auto addIfDelimiterEdge(
        Frustum::Face const f1,
        Frustum::Face const f2,
        ConvexVolume & volume) const
        -> void
    {
        const auto edge = viewerFrustum.getEdge(f1, f2);

        const auto lightToEdge = edge.first - lightPosition;

        const auto isDelimiterEdge = 
            isPlaneFacingAwayFromLight(f1, lightToEdge) &&
            !isPlaneFacingAwayFromLight(f2, lightToEdge);

        if (isDelimiterEdge)
        {
            auto const plane = makeFrustumPlane(lightPosition, edge.first, edge.second);

            volume.addPlane(plane);
        }
    }

    auto isPlaneFacingAwayFromLight(Frustum::Face const f, glm::vec3 const & lightToEdge) const
        -> bool
    {
        auto const & plane = viewerFrustum.getPlane(f);

        return (glm::dot(lightToEdge, plane.normal) > 0.0f);
    }

private:

    glm::vec3 lightPosition;

    Frustum lightFrustum;

    Frustum viewerFrustum;

};

} // unnamed namespace

PerspectiveLightCuller::PerspectiveLightCuller(
    Camera const & lightPerspective,
    Camera const & viewerPerspective)
    : lightPerspective{&lightPerspective}
    , viewerPerspective{&viewerPerspective}
    , cullingVolume{makeCullingVolume()}
{
}

auto PerspectiveLightCuller::isSphereContained(Sphere const & sphere) const
    -> ContainmentRelation
{
    auto relation = ContainmentRelation::fullyInside;

    auto const center = sphere.getCenter();

    auto const radius = sphere.getRadius();

    for (auto const & plane : cullingVolume.getPlanes())
    {
        auto const distance = computeSignedDistance(center, plane);

        if (distance < -radius)
        {
            return ContainmentRelation::fullyOutside;
        }
        else if (distance < radius)
        {
            return ContainmentRelation::partlyInside;
        }
    }

    return relation;
}

auto PerspectiveLightCuller::isPointContained(glm::vec3 const & point) const
    -> bool
{
    auto const & planes = cullingVolume.getPlanes();

    auto const isOnPositiveSide = [&point] (Plane const & plane)
    {
        auto const signedDistance = computeSignedDistance(point, plane);

        return (signedDistance >= 0.0f);
    };

    return basix::containsIf(planes, isOnPositiveSide);
}

auto PerspectiveLightCuller::makeCullingVolume() const
    -> ConvexVolume
{
    return BoundingVolumeCalculator{*lightPerspective, *viewerPerspective}.calculate();
}

} // namespace ape
