#ifndef _MODEL_H_
#define _MODEL_H_

#include "ogl.h"
#include "texture.h"

class Model
{
	private:
		struct VertexType
		{
			float x, y, z;
			//float r, g, b;
			float tu, tv;
		};

	public:
		Model();
		Model(const Model&);
		~Model();

		bool Initialize(OGL*, char*, unsigned int, bool);
		void Shutdown(OGL*);
		void Render(OGL*);

	private:
		bool InitializeBuffers(OGL*);
		void ShutdownBuffers(OGL*);
		void RenderBuffers(OGL*);
		bool LoadTexture(OGL*, char*, unsigned int, bool);
		void ReleaseTexture();

		int m_vertexCount, m_indexCount;
		unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
		TextureClass* m_Texture;
};


#endif