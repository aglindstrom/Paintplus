#include "textureshader.h"

TextureShader::TextureShader()
{

}

TextureShader::TextureShader(const TextureShader& other)
{

}

TextureShader::~TextureShader()
{

}

bool TextureShader::Initialize(OGL* openGL, HWND hwnd)
{
	bool result;

	result = InitializeShader("texture.vs", "texture.ps", openGL, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextureShader::Shutdown(OGL* openGL)
{
	ShutdownShader(openGL);

	return;
}

void TextureShader::SetShader(OGL* openGL)
{
	openGL->glUseProgram(m_shaderProgram);

	return;
}

bool TextureShader::InitializeShader(char* vertexShader, char* pixelShader, OGL* openGL, HWND hwnd)
{
	const char* vertexShaderBuffer;
	const char* pixelShaderBuffer;
	int status;

	vertexShaderBuffer = LoadShaderSourceFile(vertexShader);
	if (!vertexShaderBuffer)
	{
		return false;
	}

	pixelShaderBuffer = LoadShaderSourceFile(pixelShader);
	if (!pixelShaderBuffer)
	{
		return false;
	}

	m_vertexShader = openGL->glCreateShader(GL_VERTEX_SHADER);
	m_pixelShader = openGL->glCreateShader(GL_FRAGMENT_SHADER);

	openGL->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	openGL->glShaderSource(m_pixelShader, 1, &pixelShaderBuffer, NULL);

	delete[] vertexShaderBuffer;
	vertexShaderBuffer = 0;

	delete[] pixelShaderBuffer;
	pixelShaderBuffer = 0;

	openGL->glCompileShader(m_vertexShader);
	openGL->glCompileShader(m_pixelShader);

	openGL->glGetShaderIV(m_vertexShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(openGL, hwnd, m_vertexShader, vertexShader);
		return false;
	}

	openGL->glGetShaderIV(m_pixelShader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(openGL, hwnd, m_pixelShader, pixelShader);
		return false;
	}

	m_shaderProgram = openGL->glCreateProgram();

	openGL->glAttachShader(m_shaderProgram, m_vertexShader);
	openGL->glAttachShader(m_shaderProgram, m_pixelShader);

	openGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
	openGL->glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");

	openGL->glLinkProgram(m_shaderProgram);

	openGL->glGetProgramIV(m_shaderProgram, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(openGL, hwnd, m_shaderProgram);
		return false;
	}

	return true;
}

char* TextureShader::LoadShaderSourceFile(char* fileName)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	fin.open(fileName);

	if (fin.fail())
	{
		return 0;
	}

	fileSize = 0;

	fin.get(input);

	while (!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	fin.close();

	buffer = new char[fileSize + 1];

	fin.open(fileName);

	fin.read(buffer, fileSize);

	fin.close();

	buffer[fileSize] = '\0';

	return buffer;
}

void TextureShader::OutputShaderErrorMessage(OGL* openGL, HWND hwnd, unsigned int shaderId, char* fileName)
{
	int logSize, i;
	char* infoLog;
	std::ofstream fout;
	wchar_t newString[128];
	unsigned int error, convertedChars;

	openGL->glGetShaderIV(shaderId, GL_INFO_LOG_LENGTH, &logSize);

	logSize++;

	infoLog = new char[logSize];
	if (!infoLog)
	{
		return;
	}

	openGL->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

	fout.open("shader-error.txt");

	for (i = 0; i < logSize; i++)
	{
		fout << infoLog[i];
	}

	fout.close();

	error = mbstowcs_s(&convertedChars, newString, 128, fileName, 128);
	if (error != 0)
	{
		return;
	}

	MessageBox(hwnd, _T("Error compiling shader. Check shader-error.txt."), fileName, MB_OK);

	return;

}

void TextureShader::OutputLinkerErrorMessage(OGL* openGL, HWND hwnd, unsigned int programId)
{
	int logSize, i;
	char* infoLog;
	std::ofstream fout;

	openGL->glGetProgramIV(programId, GL_INFO_LOG_LENGTH, &logSize);

	logSize++;

	infoLog = new char[logSize];
	if (!infoLog)
	{
		return;
	}

	openGL->glGetProgramInfoLog(programId, logSize, NULL, infoLog);

	fout.open("linker-error.txt");

	for (i = 0; i < logSize; i++)
	{
		fout << infoLog[i];
	}

	fout.close();

	MessageBox(hwnd, _T("Error compiling linker. Check linker-error.txt for message."), _T("Linker Error"), MB_OK);

	return;
}

void TextureShader::ShutdownShader(OGL* openGL)
{
	openGL->glDetachShader(m_shaderProgram, m_vertexShader);
	openGL->glDetachShader(m_shaderProgram, m_pixelShader);

	openGL->glDeleteShader(m_vertexShader);
	openGL->glDeleteShader(m_pixelShader);

	openGL->glDeleteProgram(m_shaderProgram);

	return;
}

bool TextureShader::SetShaderParameters(OGL* openGL, float* worldMatrix, float* viewMatrix, float* projectionMatrix, int texUnit)
{
	unsigned int location;

	location = openGL->glGetUniformLocation(m_shaderProgram, "worldMatrix");
	if (location == -1)
	{
		return false;
	}
	openGL->glUniformMatrix4fv(location, 1, false, worldMatrix);

	location = openGL->glGetUniformLocation(m_shaderProgram, "viewMatrix");
	if (location == -1)
	{
		return false;
	}
	openGL->glUniformMatrix4fv(location, 1, false, viewMatrix);

	location = openGL->glGetUniformLocation(m_shaderProgram, "projectionMatrix");
	if (location == -1)
	{
		return false;
	}
	openGL->glUniformMatrix4fv(location, 1, false, projectionMatrix);

	location = openGL->glGetUniformLocation(m_shaderProgram, "shaderTexture");
	if (location == -1)
	{
		return false;
	}
	openGL->glUniform1i(location, texUnit);

	return true;
}