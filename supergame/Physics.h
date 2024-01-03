#pragma once

#include "../core/include/IApplication.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

class Physics
{
public:
	Physics();

	void Update(float frametime);

	btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld.get(); }
	const btDiscreteDynamicsWorld* GetDynamicsWorld() const { return m_pDynamicsWorld.get(); }

private:
	std::unique_ptr<btBroadphaseInterface>					m_pBroadphase;
	std::unique_ptr<btDefaultCollisionConfiguration>		m_pCollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher>					m_pDispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_pSolver;
	std::unique_ptr<btDiscreteDynamicsWorld>				m_pDynamicsWorld;

};

