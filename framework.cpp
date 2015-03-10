#include "framework.h"

Framework::Framework()
{
	
}

Framework::Framework(const Framework& other)
{

}

Framework::~Framework()
{

}

bool Framework::Initialize()
{
	int width, height;
	bool result;
	
	width = 0;
	height = 0;
	
	InitializeWindows(width, height);
	
	return true;
}

void Framework::Shutdown()
{
	ShutdownWindows();
	
	return;
}

void Framework::InitializeWindows(int& width, int& height)
{
	WNDCLASSEX wcex;
	int posX, posY;
	
	ApplicationHandle = this;
	
	m_hinstance = GetModuleHandle(NULL);
	
	m_appName = "Paint+";
	
	wcex.cbSize 			= sizeof(WNDCLASSEX);
	wcex.style 				= CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wcex.lpfnWndProc 		= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance 		= m_hinstance;
	wcex.hIcon				= LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName		= NULL;
	wcex.lpszClassName	= m_appName;
	wcex.hIconSm			= wcex.hIcon;
	
	if(!RegisterClassEx(&wcex))
	{
   	return;
	}
	
	width  = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	
	posX = (GetSystemMetrics(SM_CXSCREEN) - width)/2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - height)/2;
	
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
									m_appName, 
									m_appName, 
									WS_OVERLAPPEDWINDOW,
									posX, posY,
									width, height,
									NULL, NULL, 
									m_hinstance,
									NULL);
									
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	
	ShowCursor(true);
	
	return;	
}

void Framework::ShutdownWindows()
{
	ShowCursor(true);
	
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
	
	UnregisterClass(m_appName, m_hinstance);
	m_hinstance = NULL;
	
	ApplicationHandle = NULL;
	
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, message, wparam, lparam);
		}
	
	}
}

LRESULT CALLBACK Framework::MessageHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			return 0;
		}
		case WM_KEYUP:
		{
			return 0;
		}
		
		default:
		{
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}
}











