#ifndef _IMAGELIST_H_
#define _IMAGELIST_H_

#include <string.h>

class Imagelist
{
	struct imageNode{
		char* filename;
		unsigned char* texture;
		unsigned int index;
		int width, height, size;
		
		imageNode* next;
		imageNode* prev;
	};

	public:
		Imagelist();
		Imagelist(const Imagelist&);
		~Imagelist();

		bool Initialize(char*);
		bool addTexture(char*);
		void Shutdown();

		void getFileName(char*);
		void getImage(unsigned char*, int);
		int getWidth(){ return current->width; }
		int getHeight(){ return current->height; }
		int getSize(){ return current->size; }
		int getIndex(){ return current->index; }

		void setCurrent(int);
		void setImage(unsigned char*, int);
		void setWidth(int width){ current->width = width; }
		void setHeight(int height){ current->height = height; }
		void setSize(int size){ current->size = size; }

		bool newImage(int size){ current->texture = new unsigned char[size]; if (!current->texture){ return false; } return true; }

	private:

		imageNode* first;
		imageNode* last;
		imageNode* current;

		void deleteNode(int);
		bool addNode(char*);
		void modifyImage(unsigned char*, int);
};

#endif