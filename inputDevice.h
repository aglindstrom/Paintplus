#ifndef _INPUTDEVICE_H_
#define _INPUTDEVICE_H_

#include <windows.h>

class inputDevice
{
	public:
		inputDevice();
		inputDevice(const inputDevice&);
		~inputDevice();
		
		bool Initialize();
		void KeyDown(unsigned int);
		void KeyUp(unsigned int);
		bool IsKeyDown(unsigned int);
		void GetMouseLoc(int&, int&);
		
	private:
		bool m_keys[256];
};

#endif