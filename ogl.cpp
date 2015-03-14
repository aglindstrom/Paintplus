#include "ogl.h"

OGL::OGL()
{

}

OGL::OGL(const OGL& other)
{

}

OGL::~OGL()
{

}

bool OGL::InitializeExtensions(HWND hwnd)
{
	HDC deviceContext;
	PIIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;

	deviceContext = GetDC(hwnd);
	if(!deviceContext)
	{
		return false;
	}

	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if(error != 1)
	{
		return false;
	}

	renderContext = wglCreateContext(deviceContext);
	if(!renderContext)
	{
		return false;
	}

	error = wglMakeCurrent(deviceContext, renderContext);
	if(error != 1)
	{
		return false;
	}

	result = LoadExtensionList();
	if(!result)
	{
		return false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = NULL;

	ReleaseDC(hwnd, deviceContext);
	deviceContext = 0;

	return true;
}

bool OGL::InitializeOpenGL(HWND hwnd, int width, int height, float depth, float near, bool vSync)
{
	int attribListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int attribList[5];
	float FOV, Aspect;
	char *vendorString, *renderString;

	m_deviceContext = GedDC(hwnd);
	if(!m_deviceContext)
	{
		return false;
	}

	attribListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attribListInt[1] = TRUE;

	attribListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attribListInt[3] = TRUE;

	attribListInt[4] = WGL_ACCELERATION_ARB;
	attribListInt[5] = WGL_FULL_ACCELERATION_ARB;

	attribListInt[6] = WGL_COLOR_BITS_ARB;
	attribListInt[7] = 24;

	attribListInt[8] = WGL_DEPTH_BITS_ARB;
	attribListInt[9] = 24;

	attribListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attribListInt[11] = TRUE;

	attribListInt[12] = WGL_SWAP_METHOD_ARB;
	attribListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	attribListInt[14] = WGL_PIXEL_TYPE_ARB;
	attribListInt[15] = WGL_TYPE_RGA_ARB;

	attribListInt[16] = WGL_STENCIL_BITS_ARB;
	attribListInt[17] = 8;

	attribListInt[18] = 0;

	result = wglChoosePixelFormatARB(m_deviceContext, attribListInt, NULL, 1, pixelFormat, &formatCount);
	if(result != 1)
	{
		return false;
	}

	attribList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attribList[1] = 4;
	attribList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attribList[3] = 0;
	attribList[4] = 0;

	m_renderingContext = wglCreateContextAttribsArb(m_deviceContext, 0, attribList);
	if(m_renderingContext == NULL)
	{
		return false;
	}

	result = wglMakeCurrent(m_deviceContext,m_renderingContext);
	if(result != 1)
	{
		return false;
	}


	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glFrontFACE(GL_CW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	BuildIdentityMatrix(m_worldMatrix);

	FOV =  3.14159265358979323846f / 4.0f;
	Aspect = (float)width / (float) height;

	BuildPerspectivFovLHMatrix(m_projectionMatrix, FOV, Aspect, near, depth);

	vendorString = (char*)glGetString(GL_VENDOR);
	renderString = (char*)glGetString(GL_RENDERER);

	strcpy_s(m_videoCardDescription, vendorString);
	strcat_s(m_videoCardDescription, "-");
	strcat_s(m_videoCardDescription, renderString);

	if(vSync)
	{
		result = wglSwapIntervalEXT(1);
	}
	else
	{
		resutl = wglSwapIntervalEXT(0);
	}

	if(result != 1)
	{
		return false;
	}

	return true;
}

void OGL::Shutdown(HWND hwnd)
{
	if(m_renderingContext)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_renderingContext);
		m_renderingContext = 0;

	}

	if(m_deviceContext)
	{
		ReleaseDC(hwnd, m_deviceContext);
		m_deviceContext = 0;
	}
	return;
}

void OGL::BeginScene(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	return;

}

void OGL::EndScene()
{
	SwapBuffers(m_deviceContext);

	return;
}

bool OGL::LoadExensionList()
{
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if(!wglChoosePixelFormatARB)
	{
		return false;
	}

	wglCreateContextAttribsArb = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if(!wglCreateContextAttribsARB)
	{
		return false;
	}

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(!wglSwapIntervalEXT)
	{
		return false;
	}

	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	if(!glAttachShader)
	{
		return false;
	}

	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if(!glBindBuffer)
	{
		return false;
	}

	glBindVertexArray = (PFNGLBINDVERTEXARRAY)wglGetProcAddress("glBindVertexArray");
	if(!glBindVertexArray)
	{
		return false;
	}

	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	if(!glBufferData)
	{
		return false;
	}

	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	if(!glCompileShader)
	{
		return false;
	}

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	if(!glCreateProgram)
	{
		return false;
	}

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	if(!glCreateShader)
	{
		return false;
	}

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	if(!glDeleteBuffers)
	{
		return false;
	}

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	if(!glDeleteProgram)
	{
		return false;
	}

	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	if(!glDeleteShader)
	{
		return false;
	}

	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	if(!glDetachShader)
	{
		return false;
	}

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if(!glEnableVertexAttribArray)
	{
		return false;
	}

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if(!glGenBuffers)
	{
		return false;
	}

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	if(!glGenVertexArrays)
	{
		return false;
	}

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	if(!glGenVertexArrays)
	{
		return false;
	}

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	if(!glGetProgramInfoLog)
	{
		return false;
	}

	glGetProgramiv = (PFNGLGETPROGRMAIVPROC)wglGetProcAddress("glGetProgramiv")
	if(!glGetProgramiv)
	{
		return false;
	}

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	if(!glGetShaderInfoLog)
	{
		return false;
	}

	glGetShaderiv = (PFNGLGETSHADERIVPROV)wglGetProcAddress("glGetShaderiv");
	if(!glGetShaderiv)
	{
		return false;
	}

	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	if(!glLinkProgram)
	{
		return false;
	}

	glShaderSource = (PFNGLSHAERSOURCEPROC)wglGetProcAddress("glShaderSource");
	if(!glShaderSource)
	{
		return false;
	}

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgam");
	if(!glUseProgam)
	{
		return false;
	}

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if(!glVertexAttribPointer)
	{
		return false;
	}

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	if(!glBindAttribLocation)
	{
		return false;
	}

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATION)wglGetProcAddress("glGetUniformLocation");
	if(!glGetUniformLocation)
	{
		return false;
	}

	glUniformMatrix4fv = (PFNGLUNIRMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	if(!glUniformMatrix4fv)
	{
		return false;
	}

	glActiveTexture = (PFNGLACTIVETEXTURE)wglGetProcAddress("glActiveTexture");
	if(!glActiveTexture)
	{
		return false;
	}

	
}