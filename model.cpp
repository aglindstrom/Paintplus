#include "model.h"

Model::Model()
{
	m_Texture = 0;
}

Model::Model(const Model& other)
{

}

Model::~Model()
{

}

bool Model::Initialize(HWND hwnd, OGL* openGL, char* texFileName, unsigned int texUnit, bool wrap)
{
	bool result;
	
	m_hwnd = hwnd; 

	result = InitializeBuffers(openGL);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(openGL, texFileName, texUnit, wrap);
	if (!result)
	{
		return false;
	}


	return true;
}

void Model::Shutdown(OGL* openGL)
{
	ReleaseTexture();

	ShutdownBuffers(openGL);

	return;
}

void Model::Render(OGL* openGL)
{
	RenderBuffers(openGL);

	return;
}

bool Model::InitializeBuffers(OGL* openGL)
{
	VertexType* vertices;
	unsigned int* indices;

	m_vertexCount = 4;
	m_indexCount = 6;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		MessageBox(m_hwnd, "Could Not allocate vertex", "Error", MB_OK);
		return false;
	}

	indices = new unsigned int[m_indexCount];
	if (!indices)
	{
		MessageBox(m_hwnd, "Could Not allocate index", "Error", MB_OK);
		return false;
	}

	vertices[0].x = -1.0f;
	vertices[0].y = -1.0f;
	vertices[0].z = 0.0f;

	vertices[0].tu = 0.0f;
	vertices[0].tv = 0.0f;

	vertices[1].x = -1.0f;
	vertices[1].y = 1.0f;
	vertices[1].z = 0.0f;

	vertices[1].tu = 0.0f;
	vertices[1].tv = 1.0f;

	vertices[2].x = 1.0f;
	vertices[2].y = 1.0f;
	vertices[2].z = 0.0f;

	vertices[2].tu = 1.0f;
	vertices[2].tv = 1.0f;

	vertices[3].x = 1.0f;
	vertices[3].y = -1.0f;
	vertices[3].z = 0.0f;

	vertices[3].tu = 1.0f;
	vertices[3].tv = 0.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	openGL->glGenVertexArrays(1, &m_vertexArrayId);

	openGL->glBindVertexArray(m_vertexArrayId);

	openGL->glGenBuffers(1, &m_vertexBufferId);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	openGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	openGL->glEnableVertexAttribArray(0);
	openGL->glEnableVertexAttribArray(1);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	openGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	openGL->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

	openGL->glGenBuffers(1, &m_indexBufferId);

	openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers(OGL* openGL)
{
	openGL->glDisableVertexAttribArray(0);

	openGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
	openGL->glDeleteBuffers(1, &m_vertexBufferId);

	openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	openGL->glDeleteBuffers(1, &m_indexBufferId);

	openGL->glBindVertexArray(0);
	openGL->glDeleteVertexArrays(1, &m_vertexArrayId);

	return;
}

void Model::RenderBuffers(OGL* openGL)
{
	openGL->glBindVertexArray(m_vertexArrayId);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	
	return;
}

bool Model::LoadTexture(OGL* openGL, char* fileName, unsigned int texUnit, bool wrap)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		MessageBox(m_hwnd, "Could Not allocate Texture", "Error", MB_OK);
		return false;
	}

	result = m_Texture->Initialize(m_hwnd, openGL, fileName, texUnit, wrap);
	if (!result)
	{
		MessageBox(m_hwnd, "Could Not init Texture", "Error", MB_OK);
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}