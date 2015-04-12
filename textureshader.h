#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include <fstream>
using namespace std;

#include "ogl.h"

class TextureShader
{
	public:
		TextureShader();
		TextureShader(const TextureShader&);
		~TextureShader();
	
		bool Initialize(OGL*, HWND);
		void Shutdown(OGL*);
		void SetShader(OGL*);
		bool SetShaderParameters(OGL*, float*, float*, float*, int);

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