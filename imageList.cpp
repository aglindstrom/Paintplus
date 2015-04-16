#include "imageList.h"

Imagelist::Imagelist()
{

}

Imagelist::Imagelist(const Imagelist& other)
{

}

Imagelist::~Imagelist()
{

}

bool Imagelist::Initialize(char* fileName)
{
	first = new imageNode;
	if (!first)
	{
		return false;
	}
	current = first;
	last = first;

	first->filename = fileName;

	return true;
}

bool Imagelist::addTexture(char* fileName)
{
	if (!addNode(fileName))
	{
		return false;
	}

	return true;
}

void Imagelist::Shutdown()
{
	while (last != first){
		deleteNode(last->index);
	}

	return;
}

bool Imagelist::addNode(char* fileName)
{
	last->next = new imageNode;
	if (!last->next)
	{
		return false;
	}

	current = last;
	last = last->next;
	last->prev = current;

	last->index = current->index + 1;
	last->filename = fileName;

	current = last;

	return true;
}

void Imagelist::modifyImage(unsigned char* texture, int index)
{
	current = first;

	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}

	current->texture = texture;
}

void Imagelist::deleteNode(int index)
{
	current = first;

	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}

	current->prev->next = current->next;
	current->next->prev = current->prev;

	delete current;
	current = first;

	return;
}

void Imagelist::setCurrent(int index)
{
	int i = 0;
	current = first;

	while( current != last && i < index)
	{
		current = current->next;
		i++;
	}
}

void Imagelist::getFileName(char* filename)
{
	filename = current->filename;
	return;
}

void Imagelist::setImage(unsigned char* texture, int size)
{
	for (int i = 0; i < size; i++)
	{
		current->texture[i] = texture[i];
	}
}

void Imagelist::getImage(unsigned char* texture, int size)
{
	for (int i = 0; i < size; i++)
	{
		texture[i] = current->texture[i];
	}
}