#ifndef OPENMW_COMPONENTS_DETOURNAVIGATOR_MAKENAVMESH_H
#define OPENMW_COMPONENTS_DETOURNAVIGATOR_MAKENAVMESH_H

#include "tileposition.hpp"

#include <osg/Vec3f>

#include <memory>
#include <set>

class dtNavMesh;

namespace DetourNavigator
{
    class RecastMesh;
    struct Settings;

    using NavMeshPtr = std::shared_ptr<dtNavMesh>;

    NavMeshPtr makeEmptyNavMesh(const osg::Vec3f& agentHalfExtents, const RecastMesh& recastMesh,
            const Settings& settings);

    NavMeshPtr makeNavMesh(const osg::Vec3f& agentHalfExtents, const RecastMesh& recastMesh,
            const Settings& settings);

    void updateNavMesh(const osg::Vec3f& agentHalfExtents, const RecastMesh& recastMesh,
            const std::set<TilePosition>& changedTiles, const Settings& settings, dtNavMesh& navMesh);
}

#endif
