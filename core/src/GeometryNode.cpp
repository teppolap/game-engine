/**
 * ============================================================================
 *  Name        : GeometryNode.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Scenegraph node capable of rendering geometry object with 
 *                material.
 * ============================================================================
**/

#include "../include/GeometryNode.h"
#include "../include/Geometry.h"
#include "../include/Material.h"


void GeometryNode::Render(IRenderer& renderer, GLuint program)
{
	if (m_pGeometry)
	{
		m_pGeometry->SetAttribs(program);

		// set model matrix to shader uniform
		const glm::mat4 worldMatrix(GetWorldMatrix());
		OpenGLRenderer::SetUniformMatrix4(program, "modelMatrix", worldMatrix);

		// set model-view-projection matrix to shader uniform
		const glm::mat4 modelViewProjectionMatrix(renderer.GetProjectionMatrix() * renderer.GetViewMatrix() * worldMatrix);
		OpenGLRenderer::SetUniformMatrix4(program, "modelViewProjectionMatrix", modelViewProjectionMatrix);

		if (m_pMaterial)
		{
			m_pMaterial->SetToProgram(program);
		}

		m_pGeometry->Draw(renderer);
	}

	Node::Render(renderer, program);
}

