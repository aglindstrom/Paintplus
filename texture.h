#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdio.h>

#include "ogl.h"
#include "imageList.h"

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

		bool Initialize(HWND, OGL*, char*, bool);
		bool addTexture(HWND, OGL*, char*, bool);
		void Shutdown();
		void ModifyTexture(OGL*, unsigned int, int, int);

	private:
		bool LoadTarga(OGL*, char*, unsigned int, bool);
		void attatchImage(OGL*, int);

		bool loaded;
		unsigned int m_textureID;
		int m_width, m_height;
		Imagelist* m_images;
		HWND m_hwnd;
};

#endif