#include "output.h"

output::output()
{
	m_openGL = 0;
	m_camera = 0;
	m_model = 0;
	m_TextureShader = 0;
}

output::output(const output& other)
{

}

output::~output()
{

}

bool output::Initialize(OGL* ogl, HWND hwnd)
{
	m_openGL = ogl;

	m_camera = new Camera;
	if (!m_camera)
	{
		MessageBox(hwnd, "Could Not allocate Camera", "Error", MB_OK);
		return false;
	}

	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_model = new Model;
	if (!m_model)
	{
		MessageBox(hwnd, "Could Not allocate Model", "Error", MB_OK);
		return false;
	}

	if (!m_model->Initialize(hwnd, m_openGL, "./test.tga", 0, true))
	{
		MessageBox(hwnd, "Could Not init Model", "Error", MB_OK);
		return false;
	}

	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		MessageBox(hwnd, "Could Not allocate Tex Shader", "Error", MB_OK);
		return false;
	}

	if (!m_TextureShader->Initialize(m_openGL, hwnd))
	{
		MessageBox(hwnd, "Could Not init Tex Shader", "Error", MB_OK);
		return false;
	}

	/*
	m_shader = new ColorShader;
	if (!m_shader)
	{
		return false;
	}

	if (!m_shader->Initialize(m_openGL, hwnd))
	{
		return false;
	}
	*/
	return true;
}

bool output::Frame()
{
	bool result;

	result = Render();
	if(!result)
	{
		return false;
	}

	//m_camera->Rotate(1.0f, 2.0f, 3.0f);

	return true;
}

void output::Shutdown()
{

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown(m_openGL);
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_model)
	{
		m_model->Shutdown(m_openGL);
		delete m_model;
		m_model = 0;
	}

	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	m_openGL = 0;

	return;
}

bool output::Render()
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	m_openGL->BeginScene(0.4666f,0.4666f,0.4666f,1.0f);

	m_camera->Render();

	m_openGL->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_openGL->GetProjectionMatrix(projectionMatrix);

	m_TextureShader->SetShader(m_openGL);
	m_TextureShader->SetShaderParameters(m_openGL, worldMatrix, viewMatrix, projectionMatrix, 0);

	m_model->Render(m_openGL);

	m_openGL->EndScene();

	return true;
}
