#pragma once
#include "../core/include/IApplication.h"
#include "../core/include/Geometry.h"
#include "../core/include/Material.h"
#include "../core/include/GeometryNode.h"
#include "../core/include/CameraNode.h"

// physics
#include "Physics.h"


class TheApp : public IApplication
{
public:
	TheApp();

	bool OnCreate() override;
	void OnDestroy() override;

	void OnUpdate(float frametime) override;
	void OnDraw(IRenderer& renderer) override;

	bool OnKeyDown(uint32_t keyCode) override;

private:
	void OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels) override;
	bool OnMouseBegin(int32_t buttonIndex, const glm::vec2& point) override;
	bool OnMouseDrag(int32_t buttonIndex, const glm::vec2& point) override;
	bool OnMouseEnd(int32_t buttonIndex, const glm::vec2& point) override;

	OpenGLRenderer* GetOpenGLRenderer() { return static_cast<OpenGLRenderer*>(GetRenderer()); }


	GLuint						m_uVertexShader;
	GLuint						m_uFragmentShader;
	GLuint						m_uProgram;

	GLuint						m_uTexture;

	std::shared_ptr<Geometry>	m_pGeometry;
	std::shared_ptr<Material>	m_pMaterial;

	std::unique_ptr<Node>		m_pSceneRoot;

	std::shared_ptr<Physics>	m_pPhysics;
};

