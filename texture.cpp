#include "texture.h"

TextureClass::TextureClass()
{
	loaded = false;
}

TextureClass::TextureClass(const TextureClass& other)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Initialize(HWND hwnd, OGL* openGL, char* filename, bool wrap)
{
	bool result;

	m_hwnd = hwnd;

	m_images = new Imagelist;
	if (!m_images)
	{
		return false;
	}

	result = m_images->Initialize(filename);
	if (!result)
	{
		return false;
	}

	result = LoadTarga(openGL, filename, 0, wrap);
	if (!result)
	{
		MessageBox(m_hwnd, "Could Not Load Targa", "Error", MB_OK);
		return false;
	}

	attatchImage(openGL, 0);

	return true;
}

bool TextureClass::addTexture(HWND hwnd, OGL* openGL, char* fileName, bool wrap)
{
	if (!m_images->addTexture(fileName))
	{
		return false;
	}

	if (!LoadTarga(openGL, fileName, m_images->getIndex() , wrap))
	{
		return false;
	}

	attatchImage(openGL, m_images->getIndex());

	return true;
}

void TextureClass::Shutdown()
{
	if (loaded)
	{
		glDeleteTextures(1, &m_textureID);
		loaded = false;
	}
	return;
}

bool TextureClass::LoadTarga(OGL* openGL, char* fileName, unsigned int texUnit, bool wrap)
{
	int error, width, height, bpp, imageSize;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
	{
		MessageBox(m_hwnd, "No Such File or Directory", "Error", MB_OK);
		return false;
	}

	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		MessageBox(m_hwnd, "File Empty", "Error", MB_OK);
		return false;
	}

	width = (int)targaFileHeader.width;
	height = (int)targaFileHeader.height;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
	{
		MessageBox(m_hwnd, "Not 32 Bit", "Error", MB_OK);
		return false;
	}

	imageSize = width * height * 4;
	targaImage = new unsigned char[imageSize];
	
	if (!m_images->newImage(imageSize))
	{
		MessageBox(m_hwnd, "Could Not allocate Targa", "Error", MB_OK);
		return false;
	}

	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		MessageBox(m_hwnd, "Read Fault", "Error", MB_OK);
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		MessageBox(m_hwnd, "Bad Close", "Error", MB_OK);
		return false;
	}

	
	m_images->setWidth(width);
	m_images->setHeight(height);
	if (!m_images->newImage(imageSize))
	{
		MessageBox(m_hwnd, "Could Not allocate Targa", "Error", MB_OK);
		return false;
	}
	m_images->setImage(targaImage, imageSize);
	m_images->setSize(imageSize);

	delete[] targaImage;
	targaImage = 0;

	loaded = true;

	return true;
}

void TextureClass::attatchImage(OGL* openGL, int index)
{
	unsigned char* texture = 0;

	m_images->setCurrent(index);
	
	texture = new unsigned char[m_images->getSize()];
	m_images->getImage(texture, m_images->getSize());

	openGL->glActiveTexture(GL_TEXTURE0 + index);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_images->getWidth(), m_images->getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	openGL->glGenerateMipmap(GL_TEXTURE_2D);

	delete[] texture;
	texture = 0;
}

void TextureClass::ModifyTexture(OGL* openGL, unsigned int texUnit, int width, int height){
	unsigned char* targaImage = 0;

	openGL->glActiveTexture(GL_TEXTURE0 + texUnit);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	openGL->glGenerateMipmap(GL_TEXTURE_2D);
}