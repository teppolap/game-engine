/**
 * ============================================================================
 *  Name        : CameraNode.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Scenegraph camera node.
 * ============================================================================
**/

#include "../include/CameraNode.h"


CameraNode::CameraNode()
{
	SetProjectionParams(glm::half_pi<float>(), 1.0f, 1.0f, 500.0f);
}

CameraNode::CameraNode(float fov, float aspect, float nearplane, float farplane)
{
	SetProjectionParams(fov, aspect, nearplane, farplane);
}


void CameraNode::Update(float frametime)
{
	Node::Update(frametime);
}


void CameraNode::Render(IRenderer& renderer, GLuint program)
{
	Node::Render(renderer, program);
}


void CameraNode::SetProjectionParams(float fov, float aspect, float nearplane, float farplane)
{
	m_mProjection = glm::perspective(fov, aspect, nearplane, farplane);

	m_fFov = fov;
	m_fAspect = aspect;
	m_fNearplane = nearplane;
	m_fFarplane = farplane;
}


glm::vec4 CameraNode::GetProjectionParams() const
{
	return glm::vec4(m_fFov, m_fAspect, m_fNearplane, m_fFarplane);
}




