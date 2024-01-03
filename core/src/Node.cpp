/**
 * ============================================================================
 *  Name        : Node.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : SceneGraph node base class
 * ============================================================================
**/

#include "../include/Node.h"

Node::Node() :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_vRotationAxis(0.0f, 0.0f, -1.0f),
	m_fRotationAngle(0.0f),
	m_fRotationSpeed(0.0f),
	m_vVelocity(0.0f),
	m_fRadius(1.0f)
{
}

Node::Node(const std::string_view& name) :
	m_mModel(1.0f),
	m_pParent(nullptr),
	m_vRotationAxis(0.0f, 0.0f, -1.0f),
	m_fRotationAngle(0.0f),
	m_fRotationSpeed(0.0f),
	m_vVelocity(0.0f),
	m_fRadius(1.0f),
	m_strName(name)
{
}


Node::~Node()
{
}


void Node::AddNode(std::shared_ptr<Node> node)
{
	// link new child parent
	node->m_pParent = this;

	// add to child array
	m_arrNodes.push_back(node);
}


void Node::Update(float frametime)
{
	// update position per velocity
	auto pos = GetPos();
	pos += m_vVelocity * frametime;

	// update rotations
	if (m_fRotationSpeed != 0.0f)
	{
		m_mModel = glm::rotate(glm::mat4(1.0f), m_fRotationAngle, m_vRotationAxis);

		m_fRotationAngle += m_fRotationSpeed * frametime;
		constexpr float pi2 = glm::two_pi<float>();
		while (m_fRotationAngle > pi2) m_fRotationAngle -= pi2;
		while (m_fRotationAngle < -pi2) m_fRotationAngle += pi2;
	}

	// set updated position back to the model matrix
	SetPos(pos);

	// update child nodes
	for (auto& node : m_arrNodes)
	{
		node->Update(frametime);
	}
}


void Node::Render(IRenderer& renderer, GLuint program)
{
	for (auto& node : m_arrNodes)
	{
		node->Render(renderer, program);
	}
}


Node* Node::FindNode(const std::string_view& name)
{
	if (GetName() == name) return this;

	for (const auto& node : m_arrNodes)
	{
		auto n = node->FindNode(name);
		if (n)
		{
			return n;
		}
	}

	return nullptr;
}

