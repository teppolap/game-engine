/**
 * ============================================================================
 *  Name        : CameraNode.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Scenegraph camera node.
 * ============================================================================
**/

#pragma once

#include "../include/Node.h"


class CameraNode : public Node
{
public:
	CameraNode();
	CameraNode(float fov, float aspect, float nearplane, float farplane);
	CameraNode(const glm::vec4& parameters);

	/*
	 * Update
	 * update node and its children
	 * @param frametime frame delta time
	 */
	void Update(float frametime) override;

	/**
	 * Render
	 * @param renderer renderer to use
	 * @param program handle to shader program
	 */
	void Render(IRenderer& renderer, GLuint program) override;

	/**
	 * SetProjectionParams
	 * change camera projection values.
	 * This function builds camera projection matrix with horizontal fov,
	 * but does not set it automatically to rendering device
	 * @param fov field of view angle in radians
	 * @param aspect view aspect ratio
	 * @param nearplane near clipping plane
	 * @param farplane far clipping plane
	 */
	void SetProjectionParams(float fov,
		float aspect,
		float nearplane,
		float farplane);
	inline void SetProjectionParams(const glm::vec4& parameters)
	{
		SetProjectionParams(parameters.x, parameters.y, parameters.z, parameters.w);
	}

	/**
	 * GetProjectionParams
	 * get current camera projection parameters
	 * @return vec4 of fov, aspect, nearplane, farplane
	 */
	glm::vec4 GetProjectionParams() const;

	/**
	 * GetProjectionMatrix
	 * @return reference to cameras projection matrix
	 */
	inline glm::mat4& GetProjectionMatrix() { return m_mProjection; }
	inline const glm::mat4& GetProjectionMatrix() const noexcept { return m_mProjection; }

	/**
	 * SetProjectionMatrix
	 * @param mMatrix camera projection matrix to set
	 */
	inline void SetProjectionMatrix(const glm::mat4& m) { m_mProjection = m;  }

	/**
	 * GetViewMatrix
	 * @return camera view matrix
	 */
	inline glm::mat4 GetViewMatrix() const { return glm::inverse(GetWorldMatrix()); }

	/**
	 * LookAt
	 * make camera node look at
	 * @param from position from where camera is looking
	 * @param at point that camera is looking
	 */
	inline void LookAt(const glm::vec3& from, const glm::vec3& at) { m_mModel = glm::inverse(glm::lookAt(from, at, glm::vec3(0.0f, 1.0f, 0.0))); }

protected:
	// camera matrices
	glm::mat4				m_mProjection;

	//	projection parameters
	float					m_fFov;
	float					m_fAspect;
	float					m_fNearplane;
	float					m_fFarplane;
};

