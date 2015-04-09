#ifndef _MODEL_H_
#define _MODEL_H_

#include "ogl.h"

class Model
{
	private:
		struct VertexType
		{
			float x, y, z;
			float r, g, b;
		};

	public:
		Model();
		Model(const Model&);
		~Model();

		bool Initialize(OGL*);
		void Shutdown(OGL*);
		void Render(OGL*);

	private:
		bool InitializeBuffers(OGL*);
		void ShutdownBuffers(OGL*);
		void RenderBuffers(OGL*);

		int m_vertexCount, m_indexCount;
		unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
};


#endif