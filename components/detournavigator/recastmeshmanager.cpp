#include "recastmeshmanager.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace DetourNavigator
{
    RecastMeshManager::RecastMeshManager(const Settings& settings)
        : mShouldRebuild(false)
        , mMeshBuilder(settings)
    {}

    bool RecastMeshManager::addObject(std::size_t id, const btHeightfieldTerrainShape& shape, const btTransform& transform)
    {
        if (!mObjects.insert(std::make_pair(id, Object {&shape, transform})).second)
            return false;
        mShouldRebuild = true;
        return true;
    }

    bool RecastMeshManager::addObject(std::size_t id, const btConcaveShape& shape, const btTransform& transform)
    {
        if (!mObjects.insert(std::make_pair(id, Object {&shape, transform})).second)
            return false;
        mShouldRebuild = true;
        return true;
    }

    boost::optional<RecastMeshManager::Object> RecastMeshManager::removeObject(std::size_t id)
    {
        const auto object = mObjects.find(id);
        if (object == mObjects.end())
            return boost::none;
        const auto result = object->second;
        mObjects.erase(object);
        mShouldRebuild = true;
        return result;
    }

    std::shared_ptr<RecastMesh> RecastMeshManager::getMesh()
    {
        rebuild();
        return mMeshBuilder.create();
    }

    void RecastMeshManager::rebuild()
    {
        if (!mShouldRebuild)
            return;
        mMeshBuilder.reset();
        for (const auto& v : mObjects)
        {
            if (v.second.mShape->getShapeType() == TERRAIN_SHAPE_PROXYTYPE)
                mMeshBuilder.addObject(*static_cast<const btHeightfieldTerrainShape*>(v.second.mShape), v.second.mTransform);
            else if (v.second.mShape->isConcave())
                mMeshBuilder.addObject(*static_cast<const btConcaveShape*>(v.second.mShape), v.second.mTransform);
        }
        mShouldRebuild = false;
    }
}
