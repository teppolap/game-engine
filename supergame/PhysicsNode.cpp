#include "PhysicsNode.h"

#include "../core/glm-master/glm/gtc/quaternion.hpp"

void PhysicsNode::SetPhysics(std::shared_ptr<Physics>& physics, float mass)
{
	m_pPhysics = physics;

	if (m_pPhysics)
	{
		m_pShape = std::make_unique<btBoxShape>(btVector3(1.0f, 1.0f, 1.0f));
		btVector3 fallInertia(0.0f, 0.0f, 0.0f);
		m_pShape->calculateLocalInertia(mass, fallInertia);
		m_fMass = mass;

		// build init motion state
		const auto worldMatrix = GetWorldMatrix();
		const glm::quat q = glm::quat_cast(worldMatrix);
		const btQuaternion qq(q.x, q.y, q.z, q.w);
		const btVector3 pos(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
		m_pMotionState = std::make_unique<btDefaultMotionState>(btTransform(qq, pos));


	}


}
