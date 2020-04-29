#include "Placeable.h"

Placeable::Placeable()
{
	xPos = 0;
	yPos = 0;
	height = 50;
	width = 50;
}

Placeable::Placeable(int x, int y, int h, int w)
{
	xPos = x;
	yPos = y;
	height = h;
	width = w;
	rectangle = new FXRectangle(x, y, w, h);
}

FXRectangle * Placeable::get_rectangle()
{
	return rectangle;
}

void Placeable::set_xPos(int newX)
{
	xPos = newX;
	rectangle->x = newX;
}

void Placeable::set_yPos(int newY)
{
	yPos = newY;
	rectangle->y = newY;
}

void Placeable::set_width(int newW)
{
	width = newW;
	rectangle->w = newW;
}

void Placeable::set_height(int newH)
{
	height = newH;
	rectangle->h = newH;
}

int Placeable::get_xPos()
{
	return xPos;
}

int Placeable::get_yPos()
{
	return yPos;
}

int Placeable::get_height()
{
	return height;
}

int Placeable::get_width()
{
	return width;
}

void Placeable::save(FXStream& stream)
{

	stream << xPos << yPos << height << width ;        
}