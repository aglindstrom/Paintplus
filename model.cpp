#include "model.h"

Model::Model()
{

}

Model::Model(const Model& other)
{

}

Model::~Model()
{

}

bool Model::Initialize(OGL* openGL)
{
	bool result;

	result = InitializeBuffers(openGL);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown(OGL* openGL)
{
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

	m_vertexCount = 3;
	m_indexCount = 3;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned int[m_indexCount];
	if (!indices)
	{
		return false;
	}

	vertices[0].x = -1.0f;
	vertices[0].y = -1.0f;
	vertices[0].z = 0.0f;

	vertices[0].r = 0.0f;
	vertices[0].g = 1.0f;
	vertices[0].b = 0.0f;

	vertices[1].x = 0.0f;
	vertices[1].y = 1.0f;
	vertices[1].z = 0.0f;

	vertices[1].r = 0.0f;
	vertices[1].g = 1.0f;
	vertices[1].b = 0.0f;

	vertices[2].x = 1.0f;
	vertices[2].y = -1.0f;
	vertices[2].z = 0.0f;

	vertices[2].r = 0.0f;
	vertices[2].g = 1.0f;
	vertices[2].b = 0.0f;

/*	vertices[3].x = 1.0f;
	vertices[3].y = 0.0f;
	vertices[3].z = 0.0f;
	vertices[3].r = 0.0f;
	vertices[3].g = 0.0f;
	vertices[3].b = 0.0f;
*/
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	//indices[3] = 3;

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
	openGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

	openGL->glGenBuffers(1, &m_indexBufferId);

	openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);

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