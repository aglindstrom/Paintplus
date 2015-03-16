#include "output.h"

output::output()
{
	m_openGL = 0;
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
	m_openGL->BeginScene(0.5f,0.5f,0.5f,1.0f);


	m_openGL->EndScene();

	return true;
}
