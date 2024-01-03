#include "Physics.h"


Physics::Physics()
{
	// configure rigid body physics
	m_pBroadphase = std::make_unique<btDbvtBroadphase>();
	m_pCollisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
	m_pDispatcher = std::make_unique<btCollisionDispatcher>(m_pCollisionConfiguration.get());
	m_pSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

	m_pDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_pDispatcher.get(),
		m_pBroadphase.get(),
		m_pSolver.get(),
		m_pCollisionConfiguration.get());
	m_pDynamicsWorld->setGravity({ 0, -9.8f, 0 });
}

void Physics::Update(float frametime)
{
	if (m_pDynamicsWorld)
	{
		m_pDynamicsWorld->stepSimulation(frametime);
	}
}

