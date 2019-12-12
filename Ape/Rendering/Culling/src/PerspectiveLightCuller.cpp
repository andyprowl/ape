#include <Ape/Rendering/Culling/PerspectiveLightCuller.hpp>

#include "CameraFrustum.hpp"

#include <Ape/Rendering/Culling/Frustum.hpp>
#include <Ape/Rendering/Culling/PlaneFrustumCuller.hpp>

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
    auto const normal = glm::normalize(glm::cross((p2 - p1), (p3 - p1)));
    
    auto const offset = glm::dot(-p1, normal);

    return {normal, offset};
}

class BoundingVolumeCalculator
{

public:

    BoundingVolumeCalculator(
        Camera const & lightPerspective,
        Camera const & viewerPerspective)
        : lightPosition{lightPerspective.getView().getPosition()}
        , lightDirection{lightPerspective.getView().getDirection()}
        , lightFrustum{extractCameraFrustum(lightPerspective)}
        , viewerFrustum{extractCameraFrustum(viewerPerspective)}
    {
    }

    auto calculate() const
        -> ConvexVolume
    {
        if (areFrustaSeparated())
        {
            return {};
        }

        return calculateTightCullingVolume();
    }

private:

    auto areFrustaSeparated() const
        -> bool
    {
        auto const frustumCuller = PlaneFrustumCuller{lightFrustum};

        return frustumCuller.isFrustumFullyOutside(viewerFrustum);
    }

    auto calculateTightCullingVolume() const
        -> ConvexVolume
    {
        auto volume = ConvexVolume{};

        addViewerPerspectiveBounds(volume);

        addLightPerspectiveBounds(volume);

        return volume;
    }

    auto addViewerPerspectiveBounds(ConvexVolume & volume) const
        -> void
    {
        auto numOfBoundaryPlanesAdded = 0;

        for (auto const face : getAllFaces())
        {
            addFacePlaneIfOpposingLight(face, volume);

            if (numOfBoundaryPlanesAdded < 6u)
            {
                numOfBoundaryPlanesAdded += addFittingPlanesInvolvingFace(face, volume);
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

    auto addFacePlaneIfOpposingLight(Frustum::Face const f, ConvexVolume & volume) const
        -> void
    {
        auto const & plane = viewerFrustum.getPlane(f);

        auto const vertexOnFace = viewerFrustum.getVertex(ape::getFace(f).front());

        auto const lightToFace = vertexOnFace - lightPosition;

        auto const isOpposing = (glm::dot(plane.normal, lightToFace) < 0.0f);

        if (isOpposing)
        {
            volume.addPlane(plane);
        }
    }

    auto addFittingPlanesInvolvingFace(Frustum::Face const face, ConvexVolume & volume) const
        -> int
    {
        auto numOfBoundaryPlanesAdded = 0;

        for (auto const neighborFace : getNeighborFaces(face))
        {
            auto const edge = viewerFrustum.getEdge(face, neighborFace);

            if (isBoundaryEdge(face, neighborFace, edge))
            {
                auto const plane = makeFrustumPlane(lightPosition, edge.first, edge.second);

                volume.addPlane(plane);
                
                ++numOfBoundaryPlanesAdded;
            }
        }

        return numOfBoundaryPlanesAdded;
    }

    auto isBoundaryEdge(
        Frustum::Face const f1,
        Frustum::Face const f2,
        std::pair<glm::vec3, glm::vec3> const & edge) const
        -> bool
    {
        auto const lightToEdge = edge.first - lightPosition;

        if (!isPlaneFacingAwayFromLight(f1, lightToEdge))
        {
            return false;
        }

        return !isPlaneFacingAwayFromLight(f2, lightToEdge);
    }

    auto isPlaneFacingAwayFromLight(Frustum::Face const f, glm::vec3 const & lightToEdge) const
        -> bool
    {
        auto const & plane = viewerFrustum.getPlane(f);

        return (glm::dot(lightToEdge, plane.normal) > 0.0f);
    }

private:

    glm::vec3 lightPosition;

    glm::vec3 lightDirection;

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

auto PerspectiveLightCuller::isCullingVolumeEmpty() const
    -> bool
{
    return cullingVolume.getPlanes().empty();
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
            relation = ContainmentRelation::partlyInside;
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
