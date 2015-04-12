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

bool Framework::Initialize(std::ofstream* errorOut)
{
	int width, height;
	bool result = true;
	
	width = 0;
	height = 0;

	keyboard_mouse = new inputDevice;
	result = keyboard_mouse->Initialize();
	if(!result)
	{
		MessageBox(m_hwnd, _T("input failed"), _T("it didn't work"), MB_OK);
		return false;
	}

	m_openGL = new OGL;
	if (!m_openGL)
	{
		return false;
	}

	InitializeWindows(m_openGL, width, height, errorOut);

	m_output = new output;
	result = m_output->Initialize(m_openGL, m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, _T("output failed"), _T("it didn't work"), MB_OK);
		return false;
	}

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

	if(m_output)
	{
		m_output->Shutdown();
		delete m_output;
		m_output = 0;
	}

	ShutdownWindows();
	
	return;
}

void Framework::Run()
{
	MSG msg;
	bool done = false;

	MessageBox(m_hwnd, _T("RUNNING"), _T("RUNNING"), MB_OK);

	ZeroMemory(&msg, sizeof(MSG));

	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			if(!Frame())
			{
				done = true;
			}
		}
	}
	return;
}

bool Framework::Frame()
{
	if(keyboard_mouse->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if(!m_output->Frame())
	{
		return false;
	}

	return true;
}

bool Framework::InitializeWindows(OGL* openGL, int& width, int& height, std::ofstream* errorOut) // Craft a window with open gl interface
{
	WNDCLASSEX wcex;
	int posX, posY;
	bool result = true;
	
	ApplicationHandle = this;
	
	m_hinstance = GetModuleHandle(NULL);
	
	m_appName = L"Paint+";
	
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
	
	result = RegisterClassEx(&wcex);
	if(!result)
	{
   		return false;
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
	*errorOut << "first" << GetLastError() << std::endl;
	ShowWindow(m_hwnd, SW_HIDE);

	result = m_openGL->InitializeExtensions(m_hwnd); // Set up the open GL Extensions (tells the computer how to use OGL)
	if(!result) // just in case the set up didn't work
	{
		MessageBox(m_hwnd, _T("Could not Initialize OpenGL extensions"), _T("It didn't work"), MB_OK);
		return false;
	}

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	posX = 0;
	posY = 0;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
									m_appName, 
									m_appName, 
									WS_POPUP,
									posX,posY,
									width, height,
									NULL, NULL, 
									m_hinstance,
									NULL);
	*errorOut << "second" << GetLastError() << std::endl;
	if(!m_hwnd)
	{
		return false;
	} // recreate window to begin OGL Initialization

	result = m_openGL->InitializeOGL(m_hwnd, width, height, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED); // Set up open GL (Use OGL) 
	if(!result)
	{
		MessageBox(m_hwnd, _T("Could Not Initialize OpenGL, check OpenGL 4.0 support"), _T("It didn't work"), MB_OK);
		return false;
	}



	ShowWindow(m_hwnd, SW_SHOW);
	*errorOut << "third" << GetLastError() << std::endl;
	SetForegroundWindow(m_hwnd);
	*errorOut << "fourth" << GetLastError() << std::endl;
	//SetFocus(m_hwnd);
	*errorOut << "fifth" << GetLastError() << std::endl;
	
	return true;	
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
			return 0;
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











