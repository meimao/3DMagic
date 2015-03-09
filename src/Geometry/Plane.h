#ifndef MAGIC3D_PLANE_H
#define MAGIC3D_PLANE_H

#include <Mesh/TriangleMesh.h>
#include <CollisionShapes\CollisionShape.h>
#include <CollisionShapes\PlaneCollisionShape.h>
#include <Mesh\TriangleMeshBuilder.h>
#include <Geometry\Geometry.h>
#include <Util\Units.h>

namespace Magic3D
{


class Plane : public Geometry
{
    Vector3 normal;
    Scalar distance;

    mutable std::shared_ptr<CollisionShape> collisionShape;
    mutable std::shared_ptr<TriangleMesh> triangleMesh;

public:
    inline Plane(const Vector3& normal, Scalar distance = 0.0f) :
        normal(normal), distance(distance) {}

    virtual const CollisionShape& getCollisionShape() const
    {
        if (collisionShape == nullptr)
            collisionShape = std::make_shared<PlaneCollisionShape>(normal, distance);
        return *this->collisionShape;
    }

    virtual const TriangleMesh& getTriangleMesh() const
    {
        throw_MagicException("Using a infinite plane as a graphical mesh is not currently supported");
    }

};

};


#endif