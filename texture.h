#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>

#include "ogl.h"

class TextureClass
{
	private:

		struct TargaHeader
		{
			unsigned char data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};

	public:
		TextureClass();
		TextureClass(const TextureClass&);
		~TextureClass();

		bool Initialize(OGL*, char*, unsigned int, bool);
		void Shutdown();

	private:
		bool LoadTarga(OGL*, char*, unsigned int, bool);

		bool loaded;
		unsigned int m_textureID;
};

#endif