#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#include "inputDevice.h"
#include "output.h"
#include "ogl.h"
#include <string.h>
#include <tchar.h>
#include <windows.h>

class Framework
{
	public:
		Framework();
		Framework(const Framework&);
		~Framework();
		
		bool Initialize();
		void Shutdown();
		void Run();
		
		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
		
	private:
		bool Frame();
		bool InitializeWindows(OGL*, int&, int&);
		void ShutdownWindows();
		
		inputDevice* keyboard_mouse;
		output* m_output;
		OGL* m_openGL;

		LPCSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Framework* ApplicationHandle = 0;

#endif