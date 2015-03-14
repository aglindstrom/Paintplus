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

	keyboard_mouse = new inputDevice;
	keyboard_mouse->Initialize();
	
	InitializeWindows(width, height);

	return true;
}

void Framework::Shutdown()
{
	if(keyboard_mouse)
	{
		delete keyboard_mouse;
		keyboard_mouse = 0;
	}

	if(m_openGL)
	{
		m_openGL->Shutdown(m_hwnd);
		delete m_openGL;
		m_openGL = 0;	
	}

	ShutdownWindows();
	
	return;
}

void Framework::Run()
{
	bool running = true;
	int i = 0;

	while(running)
	{
		if(keyboard_mouse->IsKeyDown(27))
		{
			running = false;
		}
		i++;
	}
}

void Framework::InitializeWindows(int& width, int& height)
{
	WNDCLASSEX wcex;
	int posX, posY;
	bool result;
	
	ApplicationHandle = this;
	
	m_hinstance = GetModuleHandle(NULL);
	
	m_appName = "Paint+";
	
	wcex.cbSize 			= sizeof(WNDCLASSEX);
	wcex.style 				= CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	wcex.lpfnWndProc 		= WndProc;
	wcex.cbClsExtra			= 0;
	wcex.cbWndExtra			= 0;
	wcex.hInstance 			= m_hinstance;
	wcex.hIcon				= LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName		= NULL;
	wcex.lpszClassName		= m_appName;
	wcex.hIconSm			= wcex.hIcon;
	
	if(!RegisterClassEx(&wcex))
	{
   		return;
	}
	
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
									m_appName, 
									m_appName, 
									WS_OVERLAPPEDWINDOW,
									0,0,
									640, 480,
									NULL, NULL, 
									m_hinstance,
									NULL);
									
	ShowWindow(m_hwnd, SW_HIDE);

	result = m_openGL->InitializeExtensions(m_hwnd);
	if(!result)
	{
		MessageBox(m_hwnd, _T("Could not Initialize OpenGL extensions"), _T("It didn't work"), MB_OK);
		return;
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	posX = 0;
	posY = 0;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
							m_appName, 
							m_appName, 
							WS_OVERLAPPEDWINDOW,
							posX, posY,
							width, height,
							NULL, NULL, 
							m_hinstance,
							NULL);
	if(m_hwnd == NULL)
	{
		return;
	}

	result = m_openGL->InitializeOGL(m_hwnd, width, height, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if(!result)
	{
		MessageBox(m_hwnd, _T("Could Not Initialize OpenGL, check OpenGL 4.0 support"), _T("It didn't work"), MB_OK);
		return;
	}

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
			keyboard_mouse->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP:
		{
			keyboard_mouse->KeyUp((unsigned int)wparam);
			return 0;
		}
		
		default:
		{
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}
}











