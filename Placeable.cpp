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
}

void Placeable::set_xPos(int newX)
{
	xPos = newX;
}

void Placeable::set_yPos(int newY)
{
	yPos = newY;
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
