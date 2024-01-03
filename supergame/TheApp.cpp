#include "TheApp.h"


TheApp::TheApp() :
	m_uVertexShader(0),
	m_uFragmentShader(0),
	m_uProgram(0),
	m_uTexture(0)
{
	RandSeed();
}


bool TheApp::OnCreate()
{
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = renderer->CreateTexture("earth.jpg");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// start the physics
	m_pPhysics = std::make_shared<Physics>();

	// generate geometry
	constexpr float radius = 0.5f;
	m_pGeometry = std::make_shared<Geometry>();
	m_pGeometry->GenSphere(
		glm::vec3(radius),
		glm::vec3(0.0f),
		24,
		24);

	// setup the material
	m_pMaterial = std::make_shared<Material>();
	m_pMaterial->m_cAmbient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
	//m_pMaterial->m_cDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	//m_pMaterial->m_cEmissive = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);


	// build the scenegraph
	m_pSceneRoot = std::make_unique<Node>();

	// create the camera
	auto camera = std::make_shared<CameraNode>();
	camera->SetName("camera");
	camera->SetProjectionParams({ 0.61f, GetAspect(), 1.0f, 500.0f });
	camera->LookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//camera->SetVelocity({ 0.0f, 0.0f, -1.0f });

	m_pSceneRoot->AddNode(camera);



	// create the scene of Geometry Objects
	for (size_t i = 0; i < 125; ++i)
	{
		auto node = std::make_shared<GeometryNode>(m_pGeometry, m_pMaterial);
		node->SetPos(glm::vec3(glm::linearRand(-5.0f, 5.0f),
			glm::linearRand(-5.0f, 5.0f),
			glm::linearRand(-5.0f, 5.0f)));

		//node->SetVelocity(node->GetPos());
		node->SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
		node->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));

		m_pSceneRoot->AddNode(node);
	}

	//m_pSceneRoot->SetVelocity(glm::vec3(0.0f, 0.0f, 5.0f));
	m_pSceneRoot->SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	m_pSceneRoot->SetRotationSpeed(0.1f);

	return true;
}


void TheApp::OnDestroy()
{
	m_pSceneRoot = nullptr;

	glDeleteTextures(1, &m_uTexture);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);
}


void TheApp::OnUpdate(float frametime)
{
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(frametime);
	}
	if (m_pPhysics)
	{
		m_pPhysics->Update(frametime);
	}
}


void TheApp::OnDraw(IRenderer& renderer)
{
	renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// render our geometry
	glUseProgram(m_uProgram);

	const glm::vec3 lightDirection(glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)));
	const glm::vec3 cameraPos(-renderer.GetViewMatrix()[3]);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);
	OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", cameraPos);

	renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

	// setup the camera matrices before rendering
	const auto* camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
	renderer.SetViewMatrix(camera->GetViewMatrix());
	renderer.SetProjectionMatrix(camera->GetProjectionMatrix());

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(renderer, m_uProgram);
	}
}


bool TheApp::OnKeyDown(uint32_t keyCode)
{
	if (keyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	return false;
}

void TheApp::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
	if (m_pSceneRoot)
	{
		auto camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
		if (camera)
		{
			auto params = camera->GetProjectionParams();
			params.y = GetAspect();
			camera->SetProjectionParams(params);
		}
	}
}

bool TheApp::OnMouseBegin(int32_t buttonIndex, const glm::vec2& point)
{
	/*
	Debug("OnMouseBegin " +
		std::to_string(buttonIndex) +
		": " +
		std::to_string((int)point.x) +
		" x " +
		std::to_string((int)point.y) +
		"\r\n");
	*/
	return false;
}

bool TheApp::OnMouseDrag(int32_t buttonIndex, const glm::vec2& point)
{
	/*
	Debug("OnMouseDrag " +
		std::to_string(buttonIndex) +
		": " +
		std::to_string((int)point.x) +
		" x " +
		std::to_string((int)point.y) +
		"\r\n");
	*/
	return false;
}

bool TheApp::OnMouseEnd(int32_t buttonIndex, const glm::vec2& point)
{
	/*
	Debug("OnMouseEnd " +
		std::to_string(buttonIndex) +
		": " +
		std::to_string((int)point.x) +
		" x " +
		std::to_string((int)point.y) +
		"\r\n");
	*/

	return false;
}


