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
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;
	bool result;

	deviceContext = GetDC(hwnd);
	if(!deviceContext)
	{
		MessageBox(hwnd, _T("Device Context Failure"), _T("ERROR"), MB_OK);
		return false;
	}

	
	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if(error != 1)
	{
		MessageBox(hwnd, _T("PXFormat Failure"), _T("ERROR"), MB_OK);
		return false;
	}

	
	renderContext = wglCreateContext(deviceContext);
	if(!renderContext)
	{
		MessageBox(hwnd, _T("Render Context Failure"), _T("ERROR"), MB_OK);
		return false;
	}

	
	error = wglMakeCurrent(deviceContext, renderContext);
	if(error != 1)
	{
		MessageBox(hwnd, _T("Make Current Failure"), _T("ERROR"), MB_OK);
		return false;
	}

	result = LoadExtensionList();
	if(!result)
	{
		MessageBox(hwnd, _T("Load Extensions Failure"), _T("ERROR"), MB_OK);
		return false;
	}

	// release un needed temporary render context
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderContext);
	renderContext = NULL;

	// release unneeded temporary device context
	ReleaseDC(hwnd, deviceContext);
	deviceContext = 0;

	return true;
}

bool OGL::InitializeOGL(HWND hwnd, int width, int height, float screenDepth, float screenNear, bool vSync)
{
	int attribListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	int attribList[5];
	float FOV, Aspect;
	char *vendorString, *renderString;

	m_deviceContext = GetDC(hwnd);
	if(!m_deviceContext)
	{
		MessageBox(hwnd, _T("device Context Failed"), _T("Error"), MB_OK);
		return false;
	}

	MessageBox(hwnd, _T("device Context Succeeded"), _T("Success"), MB_OK);

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
	attribListInt[15] = WGL_TYPE_RGBA_ARB;

	attribListInt[16] = WGL_STENCIL_BITS_ARB;
	attribListInt[17] = 8;

	attribListInt[18] = 0;

	result = wglChoosePixelFormatARB(m_deviceContext, attribListInt, NULL, 1, pixelFormat, &formatCount);
	if(result != 1)
	{
		return false;
	}

	result = SetPixelFormat(m_deviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if(result != 1)
	{
		return false;
	}

	attribList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attribList[1] = 4;
	attribList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attribList[3] = 0;
	attribList[4] = 0;

	m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attribList);
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

	glFrontFace(GL_CW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	BuildIdentityMatrix(m_worldMatrix);

	FOV =  3.14159265358979323846f / 4.0f;
	Aspect = (float)width / (float) height;

	BuildPerspectiveFovLHMatrix(m_projectionMatrix, FOV, Aspect, screenNear, screenDepth);

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
		result = wglSwapIntervalEXT(0);
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

bool OGL::LoadExtensionList()
{
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if(!wglChoosePixelFormatARB)
	{
		//MessageBox(hwnd, _T("Choose PX Format"), _T("ERROR"), MB_OK);
		return false;
	}

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if(!wglCreateContextAttribsARB)
	{
		//MessageBox(hwnd, _T("Create Context Attribs"), _T("ERROR"), MB_OK);
		return false;
	}

	
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if(!wglSwapIntervalEXT)
	{
		//MessageBox(hwnd, _T("Swap Interval"), _T("ERROR"), MB_OK);
		return false;
	}

	
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	if(!glAttachShader)
	{
		//MessageBox(hwnd, _T("Attach Shader"), _T("ERROR"), MB_OK);
		return false;
	}

	
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	if(!glBindBuffer)
	{
		//MessageBox(hwnd, _T("Bind Buffer"), _T("ERROR"), MB_OK);
		return false;
	}

	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	if(!glBindVertexArray)
	{
		//MessageBox(hwnd, _T("Bind Vertex Array"), _T("ERROR"), MB_OK);
		return false;
	}

	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	if(!glBufferData)
	{
		//MessageBox(hwnd, _T("Buffer Data"), _T("ERROR"), MB_OK);
		return false;
	}

	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	if(!glCompileShader)
	{
		//MessageBox(hwnd, _T("Compile Shader"), _T("ERROR"), MB_OK);
		return false;
	}

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	if(!glCreateProgram)
	{
		//MessageBox(hwnd, _T("Create Program"), _T("ERROR"), MB_OK);
		return false;
	}

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	if(!glCreateShader)
	{
		//MessageBox(hwnd, _T("Create Shader"), _T("ERROR"), MB_OK);
		return false;
	}

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	if(!glDeleteBuffers)
	{
		//MessageBox(hwnd, _T("Delete Buffers"), _T("ERROR"), MB_OK);
		return false;
	}

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	if(!glDeleteProgram)
	{
		//MessageBox(hwnd, _T("Delete Program"), _T("ERROR"), MB_OK);
		return false;
	}

	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	if(!glDeleteShader)
	{
		//MessageBox(hwnd, _T("Delete Shader"), _T("ERROR"), MB_OK);
		return false;
	}

	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	if(!glDetachShader)
	{
		//MessageBox(hwnd, _T("Detach Shader"), _T("ERROR"), MB_OK);
		return false;
	}

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	if(!glEnableVertexAttribArray)
	{
		//MessageBox(hwnd, _T("Enable Vertex Attrib Array"), _T("ERROR"), MB_OK);
		return false;
	}

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	if(!glGenBuffers)
	{
		//MessageBox(hwnd, _T("Gen Buffers"), _T("ERROR"), MB_OK);
		return false;
	}

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	if(!glGenVertexArrays)
	{
		//MessageBox(hwnd, _T("Gen Vertex Arrays"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	if(!glGenVertexArrays)
	{
		//MessageBox(hwnd, _T("Get Attrib Location"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	if(!glGetProgramInfoLog)
	{
		//MessageBox(hwnd, _T("Get Program Info Log"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetProgramIV = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	if(!glGetProgramIV)
	{
		//MessageBox(hwnd, _T("CGet Program IV"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	if(!glGetShaderInfoLog)
	{
		//MessageBox(hwnd, _T("Get Shader Info Log"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetShaderIV = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	if(!glGetShaderIV)
	{
		//MessageBox(hwnd, _T("Get Shader IV"), _T("ERROR"), MB_OK);
		return false;
	}

	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	if(!glLinkProgram)
	{
		//MessageBox(hwnd, _T("Link Program"), _T("ERROR"), MB_OK);
		return false;
	}

	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	if(!glShaderSource)
	{
		//MessageBox(hwnd, _T("Shader Source"), _T("ERROR"), MB_OK);
		return false;
	}

	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	if(!glUseProgram)
	{
		//MessageBox(hwnd, _T("Use Program"), _T("ERROR"), MB_OK);
		return false;
	}

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	if(!glVertexAttribPointer)
	{
		//MessageBox(hwnd, _T("CVertex Attrib Pointer"), _T("ERROR"), MB_OK);
		return false;
	}

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	if(!glBindAttribLocation)
	{
		//MessageBox(hwnd, _T("Bind Attrib Location"), _T("ERROR"), MB_OK);
		return false;
	}

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	if(!glGetUniformLocation)
	{
		//MessageBox(hwnd, _T("Get Uniform Location"), _T("ERROR"), MB_OK);
		return false;
	}

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	if(!glUniformMatrix4fv)
	{
		//MessageBox(hwnd, _T("Uniform Matrix 4 fv"), _T("ERROR"), MB_OK);
		return false;
	}

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	if(!glActiveTexture)
	{
		//MessageBox(hwnd, _T("Active Texture"), _T("ERROR"), MB_OK);
		return false;
	}

	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	if(!glUniform1i)
	{
		//MessageBox(hwnd, _T("Uniform 1i"), _T("ERROR"), MB_OK);
		return false;
	}

	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	if(!glGenerateMipmap)
	{
		//MessageBox(hwnd, _T("Generate MipMap"), _T("ERROR"), MB_OK);
		return false;
	}

	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	if(!glDisableVertexAttribArray)
	{
		//MessageBox(hwnd, _T("Disable Vertex Attrib Array"), _T("ERROR"), MB_OK);
		return false;
	}

	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	if(!glUniform3fv)
	{
		//MessageBox(hwnd, _T("Uniform 3fv"), _T("ERROR"), MB_OK);
		return false;
	}

	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	if(!glUniform4fv)
	{
		//MessageBox(hwnd, _T("Uniform 4fv"), _T("ERROR"), MB_OK);
		return false;
	}

	return true;
}

void OGL::GetWorldMatrix(float* matrix)
{
	matrix[0] = m_worldMatrix[0];
	matrix[1] = m_worldMatrix[1];
	matrix[2] = m_worldMatrix[2];
	matrix[3] = m_worldMatrix[3];

	matrix[4] = m_worldMatrix[4];
	matrix[5] = m_worldMatrix[5];
	matrix[6] = m_worldMatrix[6];
	matrix[7] = m_worldMatrix[7];

	matrix[8] = m_worldMatrix[8];
	matrix[9] = m_worldMatrix[9];
	matrix[10] = m_worldMatrix[10];
	matrix[11] = m_worldMatrix[11];

	matrix[12] = m_worldMatrix[12];
	matrix[13] = m_worldMatrix[13];
	matrix[14] = m_worldMatrix[14];
	matrix[15] = m_worldMatrix[15];

	return;
}

void OGL::GetProjectionMatrix(float* matrix)
{
	matrix[0] = m_projectionMatrix[0];
	matrix[1] = m_projectionMatrix[1];
	matrix[2] = m_projectionMatrix[2];
	matrix[3] = m_projectionMatrix[3];

	matrix[4] = m_projectionMatrix[4];
	matrix[5] = m_projectionMatrix[5];
	matrix[6] = m_projectionMatrix[6];
	matrix[7] = m_projectionMatrix[7];

	matrix[8] = m_projectionMatrix[8];
	matrix[9] = m_projectionMatrix[9];
	matrix[10] = m_projectionMatrix[10];
	matrix[11] = m_projectionMatrix[11];

	matrix[12] = m_projectionMatrix[12];
	matrix[13] = m_projectionMatrix[13];
	matrix[14] = m_projectionMatrix[14];
	matrix[15] = m_projectionMatrix[15];

	return;
}

void OGL::GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	return;
}

void OGL::BuildIdentityMatrix(float* matrix)
{
	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}

void OGL::BuildPerspectiveFovLHMatrix(float* matrix, float FOV, float Aspect, float screenNear, float screenDepth)
{
	matrix[0] = 1.0f/(Aspect*tan(FOV*0.5f));
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f/tan(FOV*0.5f);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = screenDepth/(screenDepth-screenNear);
	matrix[11] = 1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (-screenNear*screenDepth) / (screenDepth - screenNear);
	matrix[15] = 0.0f;

	return;
}

void OGL::MatrixRotationY(float*matrix, float angle)
{
	matrix[0] = cosf(angle);
	matrix[1] = 0.0f;
	matrix[2] = -sinf(angle);
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = sinf(angle);
	matrix[9] = 0.0f;
	matrix[10] = cosf(angle);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return;
}

void OGL::MatrixTranslation(float*matrix, float x, float y, float z)
{

	matrix[0] = 1.0f;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = 1.0f;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = 1.0f;
	matrix[11] = 0.0f;

	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	matrix[15] = 1.0f;
}

void OGL::MatrixMultiply(float* result, float* matrix1, float* matrix2)
{
	result[0]  = (matrix1[0] * matrix2[0]) + (matrix1[1] * matrix2[4]) + (matrix1[2] * matrix2[8]) + (matrix1[3] * matrix2[12]);
	result[1]  = (matrix1[0] * matrix2[1]) + (matrix1[1] * matrix2[5]) + (matrix1[2] * matrix2[9]) + (matrix1[3] * matrix2[13]);
	result[2]  = (matrix1[0] * matrix2[2]) + (matrix1[1] * matrix2[6]) + (matrix1[2] * matrix2[10]) + (matrix1[3] * matrix2[14]);
	result[3]  = (matrix1[0] * matrix2[3]) + (matrix1[1] * matrix2[7]) + (matrix1[2] * matrix2[11]) + (matrix1[3] * matrix2[15]);

	result[4]  = (matrix1[4] * matrix2[0]) + (matrix1[5] * matrix2[4]) + (matrix1[6] * matrix2[8]) + (matrix1[7] * matrix2[12]);
	result[5]  = (matrix1[4] * matrix2[1]) + (matrix1[5] * matrix2[5]) + (matrix1[6] * matrix2[9]) + (matrix1[7] * matrix2[13]);
	result[6]  = (matrix1[4] * matrix2[2]) + (matrix1[5] * matrix2[6]) + (matrix1[6] * matrix2[10]) + (matrix1[7] * matrix2[14]);
	result[7]  = (matrix1[4] * matrix2[3]) + (matrix1[5] * matrix2[7]) + (matrix1[6] * matrix2[11]) + (matrix1[7] * matrix2[15]);

	result[8]  = (matrix1[8] * matrix2[0]) + (matrix1[9] * matrix2[4]) + (matrix1[10] * matrix2[8]) + (matrix1[11] * matrix2[12]);
	result[9]  = (matrix1[8] * matrix2[1]) + (matrix1[9] * matrix2[5]) + (matrix1[10] * matrix2[9]) + (matrix1[11] * matrix2[13]);
	result[10] = (matrix1[8] * matrix2[2]) + (matrix1[9] * matrix2[6]) + (matrix1[10] * matrix2[10]) + (matrix1[11] * matrix2[14]);
	result[11] = (matrix1[8] * matrix2[3]) + (matrix1[9] * matrix2[7]) + (matrix1[10] * matrix2[11]) + (matrix1[11] * matrix2[15]);

	result[12] = (matrix1[12] * matrix2[0]) + (matrix1[13] * matrix2[4]) + (matrix1[14] * matrix2[8]) + (matrix1[15] * matrix2[12]);
	result[13] = (matrix1[12] * matrix2[1]) + (matrix1[13] * matrix2[5]) + (matrix1[14] * matrix2[9]) + (matrix1[15] * matrix2[13]);
	result[14] = (matrix1[12] * matrix2[2]) + (matrix1[13] * matrix2[6]) + (matrix1[14] * matrix2[10]) + (matrix1[15] * matrix2[14]);
	result[15] = (matrix1[12] * matrix2[3]) + (matrix1[13] * matrix2[7]) + (matrix1[14] * matrix2[11]) + (matrix1[15] * matrix2[15]);

	return;
}