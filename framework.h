#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_

#define WIN32_LEAN_AND_MEAN

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
		void InitializeWindows(int&, int&);
		void ShutdownWindows();
		
		LPCSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Framework* ApplicationHandle = 0;

#endif