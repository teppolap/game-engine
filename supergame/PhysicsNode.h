#pragma once
#include "../core/include/GeometryNode.h"
#include "Physics.h"

class PhysicsNode :
    public GeometryNode
{
public:
    void SetPhysics(std::shared_ptr<Physics>& physics, float mass);

private:
    std::shared_ptr<Physics>                m_pPhysics;

    std::unique_ptr<btCollisionShape>       m_pShape;
    std::unique_ptr<btDefaultMotionState>   m_pMotionState;
    std::unique_ptr<btRigidBody>            m_pRigidBody;

    float                                   m_fMass;
};

