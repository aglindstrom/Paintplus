#include "output.h"

output::output()
{
	m_openGL = 0;
	//m_shader = 0;
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
		return false;
	}

	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	if (!m_model->Initialize(m_openGL, "test.tga", 0, true))
	{
		return false;
	}

	m_TextureShader = new TextureShader;
	if (!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->Initialize(m_openGL, hwnd))
	{
		MessageBox(hwnd, L"Could Not init Tex Shader", L"Error", MB_OK);
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

	return true;
}

void output::Shutdown()
{
	m_openGL = 0;
}

bool output::Render()
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	m_openGL->BeginScene(0.5f,0.5f,0.5f,1.0f);

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
