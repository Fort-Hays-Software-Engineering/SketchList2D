#include "Placeable.h"

Placeable::Placeable()
{
	xPos = 0;
	yPos = 0;
	height = 50;
	width = 50;
	angle = 0;
}

Placeable::Placeable(int x, int y, int h, int w)
{
	xPos = x;
	yPos = y;
	height = h;
	width = w;
	angle = 0;
	rectangle = new FXRectangle(x, y, w, h);
}

Placeable::Placeable(int x, int y, int h, int w, int a)
{
	xPos = x;
	yPos = y;
	height = h;
	width = w;
	angle = a;
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

int Placeable::get_angle()
{
	return angle;
}

void Placeable::set_angle(int newAngle)
{
	angle = newAngle;
}

int Placeable::get_height()
{
	return height;
}

int Placeable::get_width()
{
	return width;
}

void Placeable::draw(FXDCWindow* dc)
{
	//the corners of the rectangle
	FXPoint p[4] = { FXPoint(xPos, yPos),
					 FXPoint(xPos + width, yPos),
					 FXPoint(xPos + width, yPos + height),
					 FXPoint(xPos, yPos + height) };

	//the coordinates for the center of the square
	FXPoint center = FXPoint(xPos + width * .5, yPos + height * .5);
	
	int tempX, tempY, rotatedX, rotatedY;
	
	//rotate each point
	for (int i = 0; i < 4; i++) {
		//translate to origin
		tempX = p[i].x - center.x;
		tempY = p[i].y - center.y;

		//rotate
		rotatedX = tempX*cos(angle * 3.1415 / 180) - tempY*sin(angle * 3.1415 / 180);
		rotatedY = tempX*sin(angle * 3.1415 / 180) + tempY*cos(angle * 3.1415 / 180);

		//translate back to original position
		p[i].x = rotatedX + center.x;
		p[i].y = rotatedY + center.y;
	}
	
	//draw lines betweeen each point to form a rectangle
	dc->drawLine(p[0].x, p[0].y, p[1].x, p[1].y);
	dc->drawLine(p[1].x, p[1].y, p[2].x, p[2].y);
	dc->drawLine(p[2].x, p[2].y, p[3].x, p[3].y);
	dc->drawLine(p[3].x, p[3].y, p[0].x, p[0].y);
}

void Placeable::save(FXStream& stream)
{

	stream << xPos << yPos << height << width << NULL << angle << s6 << s7;        
}