#include "inputDevice.h"

inputDevice::inputDevice()
{

}

inputDevice::inputDevice(const inputDevice&)
{

}

inputDevice::~inputDevice()
{

}

bool inputDevice::Initialize()
{
	for(int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	
	return true;
}

void inputDevice::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}

void inputDevice::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}

bool inputDevice::IsKeyDown(unsigned int input)
{
	return m_keys[input];
}

void inputDevice::GetMouseLoc(int& _x, int& _y, HWND hWnd)
{
	POINT p;
	if(GetCursorPos(&p))
	{
		_x = (int)p.x;
		_y = (int)p.y;
	}
}

