/**
 * ============================================================================
 *  Name        : GeometryNode.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Scenegraph node capable of rendering geometry object with 
 *                material.
 * ============================================================================
**/

#pragma once

#include "../include/Node.h"

// forward declarations
class Geometry;
struct Material;

class GeometryNode : public Node
{
public:
	GeometryNode(const std::shared_ptr<Geometry>& geometry,
		const std::shared_ptr<Material>& material) :
		m_pGeometry(geometry),
		m_pMaterial(material)
	{
	}

	/**
	 * Render
	 * @param renderer renderer to use
	 * @param program handle to shader program
	 */
	void Render(IRenderer& renderer, GLuint program) override;

	void SetGeometry(const std::shared_ptr<Geometry>& geometry) { m_pGeometry = geometry; }
	void SetMaterial(const std::shared_ptr<Material>& material) { m_pMaterial = material; }

protected:
	std::shared_ptr<Geometry>	m_pGeometry;
	std::shared_ptr<Material>	m_pMaterial;
};
