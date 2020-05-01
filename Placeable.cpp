#include "Placeable.h"

void Placeable::updatePoints()
{
	p[0] = FXPoint(xPos, yPos);
	p[1] = FXPoint(xPos + width, yPos);
	p[2] = FXPoint(xPos + width, yPos + height);
	p[3] = FXPoint(xPos, yPos + height);

	//the coordinates for the center of the square
	FXPoint center = FXPoint(xPos + width * .5, yPos + height * .5);

	int tempX, tempY, rotatedX, rotatedY;

	//rotate each point
	for (int i = 0; i < 4; i++) {
		//translate to origin
		tempX = p[i].x - center.x;
		tempY = p[i].y - center.y;

		//rotate
		rotatedX = tempX * cos(angle * PI / 180) - tempY * sin(angle * PI / 180);
		rotatedY = tempX * sin(angle * PI / 180) + tempY * cos(angle * PI / 180);

		//translate back to original position
		p[i].x = rotatedX + center.x;
		p[i].y = rotatedY + center.y;
	}
}

Placeable::Placeable()
{
	xPos = 0;
	yPos = 0;
	height = 50;
	width = 50;
	angle = 0;
	curgrid = 1;

	p[0] = FXPoint(xPos, yPos);
	p[1] = FXPoint(xPos + width, yPos);
	p[2] = FXPoint(xPos + width, yPos + height);
	p[3] = FXPoint(xPos, yPos + height);
}

Placeable::Placeable(int x, int y, int h, int w)
{
	xPos = x;
	yPos = y;
	height = h;
	width = w;
	angle = 0;
	rectangle = new FXRectangle(x, y, w, h);
	curgrid = 1;

	p[0] = FXPoint(xPos, yPos);
	p[1] = FXPoint(xPos + width, yPos);
	p[2] = FXPoint(xPos + width, yPos + height);
	p[3] = FXPoint(xPos, yPos + height);
}

Placeable::Placeable(int x, int y, int h, int w, int a)
{
	xPos = x;
	yPos = y;
	height = h;
	width = w;
	angle = a;
	rectangle = new FXRectangle(x, y, w, h);
	updatePoints();
	curgrid = 1;
}


FXRectangle * Placeable::get_rectangle()
{
	return rectangle;
}

void Placeable::set_xPos(int newX)
{
	xPos = newX;
	rectangle->x = newX;
	updatePoints();
}

void Placeable::set_yPos(int newY)
{
	yPos = newY;
	rectangle->y = newY;
	updatePoints();
}

void Placeable::set_width(int newW)
{
	width = newW;
	rectangle->w = newW;
	updatePoints();
}

void Placeable::set_height(int newH)
{
	height = newH;
	rectangle->h = newH;
	updatePoints();
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
	updatePoints();
}

int Placeable::get_height()
{
	return height;
}

int Placeable::get_width()
{
	return width;
}

FXint Placeable::scale(FXint x) {
	return (x * 10) / curgrid;
}
bool Placeable::isClicked(int clickX, int clickY)
{
	//rotate click to match rectangle
	int tempX, tempY, rotatedX, rotatedY;
	FXPoint center = FXPoint(xPos + width * .5, yPos + height * .5);

		//translate to origin
		tempX = clickX - scale(center.x);
		tempY = clickY - scale(center.y);

		//rotate
		rotatedX = tempX * cos(-angle * PI / 180) - tempY * sin(-angle * PI / 180);
		rotatedY = tempX * sin(-angle * PI / 180) + tempY * cos(-angle * PI / 180);

		//translate back to original position
		clickX = rotatedX + center.x;
		clickY = rotatedY + center.y;
	if (rectangle->contains(scale(clickX), scale(clickY)))
		return true;

	return false;
}

void Placeable::draw(FXDCWindow* dc, int grid)
{
	// Set divisor to determine how much each 10px x 10x grid square represents
	curgrid = grid;

	//draw lines betweeen each point to form a rectangle
	dc->drawLine(scale(p[0].x), scale(p[0].y), scale(p[1].x), scale(p[1].y));
	dc->drawLine(scale(p[1].x), scale(p[1].y), scale(p[2].x), scale(p[2].y));
	dc->drawLine(scale(p[2].x), scale(p[2].y), scale(p[3].x), scale(p[3].y));
	dc->drawLine(scale(p[3].x), scale(p[3].y), scale(p[0].x), scale(p[0].y));
}

void Placeable::drawControlHandles(FXDCWindow * dc)
{

	//get points for selection rectangle
	FXPoint select[4];
	select[0] = FXPoint(xPos-5, yPos-5);
	select[1] = FXPoint(xPos + width + 5, yPos-5);
	select[2] = FXPoint(xPos + width + 5, yPos + height + 5);
	select[3] = FXPoint(xPos -5, yPos + height + 5);

	//rotate points
	int tempX, tempY, rotatedX, rotatedY;
	FXPoint center = FXPoint(scale(xPos) + width * .5, scale(yPos) + height * .5);
	//rotate each point
	for (int i = 0; i < 4; i++) {
		//translate to origin
		tempX = select[i].x - center.x;
		tempY = select[i].y - center.y;

		//rotate
		rotatedX = tempX * cos(angle * PI / 180) - tempY * sin(angle * PI / 180);
		rotatedY = tempX * sin(angle * PI / 180) + tempY * cos(angle * PI / 180);

		//translate back to original position
		select[i].x = rotatedX + center.x;
		select[i].y = rotatedY + center.y;
	}
	//draw lines
	dc->setForeground(FXRGB(209, 209, 209));
	dc->drawLine(scale(select[0].x), scale(select[0].y), scale(select[1].x), scale(select[1].y));
	dc->drawLine(scale(select[1].x), scale(select[1].y), scale(select[2].x), scale(select[2].y));
	dc->drawLine(scale(select[2].x), scale(select[2].y), scale(select[3].x), scale(select[3].y));
	dc->drawLine(scale(select[3].x), scale(select[3].y), scale(select[0].x), scale(select[0].y));
}

void Placeable::save(FXStream& stream)
{

	stream << xPos << yPos << height << width << NULL << angle << s6 << s7;        
}

Wall::Wall(bool interior) {
	if (isInterior == true)
		return;
}

bool Wall::get_isInterior() {
	return isInterior;
}

