#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#include <fstream>
#include "ogl.h"

class ColorShader
{
	public:
		ColorShader();
		ColorShader(const ColorShader&);
		~ColorShader();

		bool Initialize(OGL*, HWND);
		void Shutdown(OGL*);
		void SetShader(OGL*);
		bool SetShaderParameters(OGL*, float*, float*, float*);

	private:
		bool InitializeShader(char*, char*, OGL*, HWND);
		char* LoadShaderSourceFile(char*);
		void OutputShaderErrorMessage(OGL*, HWND, unsigned int, char*);
		void OutputLinkerErrorMessage(OGL*, HWND, unsigned int);
		void ShutdownShader(OGL*);

		unsigned int m_vertexShader;
		unsigned int m_pixelShader;
		unsigned int m_shaderProgram;
};


#endif