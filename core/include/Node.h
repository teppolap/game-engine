/**
 * ============================================================================
 *  Name        : Node.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : SceneGraph node vase class
 * ============================================================================
**/

#pragma once

#include "../include/OpenGLRenderer.h"

class Node
{
public:
	Node();
	Node(const std::string_view& name);
	virtual ~Node();

	/*
	 * Update
	 * update node and its children
	 * @param frametime frame delta time
	 */
	virtual void Update(float frametime);

	/**
	 * Render
	 * a virtual function to render a node, base implementation is empty
	 * @param renderer renderer to use
	 * @param program handle to shader program
	 */
	virtual void Render(IRenderer& renderer, GLuint program);

	/**
	 * AddNode
	 * add new child node into the node
	 * @param node a new child to add
	 */
	void AddNode(std::shared_ptr<Node> node);

	/**
	 * GetParent
	 * @return parent node or nullptr if node has no parent
	 */
	inline Node* GetParent() { return m_pParent; }

	/**
	 * GetNodes
	 * @return child nodes
	 */
	inline auto& GetNodes() { return m_arrNodes; }
	inline const auto& GetNodes() const { return m_arrNodes; }

	/**
	 * SetPos
	 * set position into the local model matrix of this node
	 * @param pos position to set to model matrix
	 */
	inline void SetPos(const glm::vec3& pos)
	{
		m_mModel[3][0] = pos.x;
		m_mModel[3][1] = pos.y;
		m_mModel[3][2] = pos.z;
	}

	/**
	 * SetPos
	 * set position into the local model matrix of this node
	 * @param x,y,z position to set to model matrix
	 */
	inline void SetPos(float x, float y, float z)
	{
		m_mModel[3][0] = x;
		m_mModel[3][1] = y;
		m_mModel[3][2] = z;
	}

	/**
	 * GetPos
	 * @return position of the node
	 */
	inline glm::vec3 GetPos() const { return m_mModel[3]; }

	/**
	 * GetMatrix
	 * @return a reference to node local model matrix
	 */
	inline auto& GetMatrix() { return m_mModel; }

	/**
	 * SetMatrix
	 * @param m matrix to set to node
	 */
	inline void SetMatrix(const glm::mat4& m) { m_mModel = m; }

	/**
	 * GetWorldMatrix
	 * @return a matrix combined with parent
	 */
	inline glm::mat4 GetWorldMatrix() const { return (m_pParent) ? m_pParent->GetWorldMatrix() * m_mModel : m_mModel; }

	/**
	 * GetVelocity
	 * @return reference to node velocity vector
	 */
	inline auto& GetVelocity() { return m_vVelocity; }

	/**
	 * SetVelocity
	 * set node velocity vector
	 * @param velocity new velocity
	 */
	inline void SetVelocity(const glm::vec3& velocity) { m_vVelocity = velocity; }

	/**
	 * RotateAxisAngle
	 * rotate object per axis and angle. Rotation matrix is applied to the object
	 * @param axis axis to rotate around
	 * @param angle rotation angle in radians
	 */
	inline void RotateAxisAngle(const glm::vec3& axis, float angle)
	{
		m_vRotationAxis = glm::normalize(axis);
		m_fRotationAngle = angle;

		auto pos = GetPos();
		m_mModel = glm::rotate(glm::mat4(1.0f), m_fRotationAngle, m_vRotationAxis);
		SetPos(pos);
	}

	/**
	 * GetRotationAxis
	 * @return reference to current rotation axis
	 */
	inline auto& GetRotationAxis() { return m_vRotationAxis; }

	/**
	 * SetRotationAxis
	 * calls RotateAxisAngle
	 * @param axis axis to rotate around
	 */
	inline void SetRotationAxis(const glm::vec3& axis)
	{
		RotateAxisAngle(axis, m_fRotationAngle);
	}

	/**
	 * SetRotationAngle
	 * calls RotateAxisAngle
	 * @param angle rotation angle in radians
	 */
	inline void SetRotationAngle(float angle)
	{
		RotateAxisAngle(m_vRotationAxis, angle);
	}

	/**
	 * GetRotationAngle
	 * @return current rotation angle in radians
	 */
	inline float GetRotationAngle() const { return m_fRotationAngle; }

	/**
	 * SetRotationSpeed
	 * @param speed rotation speed in radians per second
	 */
	inline void SetRotationSpeed(float speed) { m_fRotationSpeed = speed; }

	/**
	 * GetRotationSpeed
	 * @return current rotation speed in radians per second
	 */
	inline float GetRotationSpeed() const { return m_fRotationSpeed; }

	inline float GetRadius() const { return m_fRadius; }
	inline void SetRadius(float radius) { m_fRadius = radius; }

	inline const std::string& GetName() const { return m_strName; }
	inline void SetName(const std::string_view name) { m_strName = name; }

	Node* FindNode(const std::string_view& name);

protected:
	glm::mat4									m_mModel;
	Node*										m_pParent;
	std::vector<std::shared_ptr<Node>>			m_arrNodes;

	// velocity and rotations
	glm::vec3									m_vVelocity;

	glm::vec3									m_vRotationAxis;
	float										m_fRotationAngle;
	float										m_fRotationSpeed;

	// size
	float										m_fRadius;

private:
	std::string									m_strName;
};

