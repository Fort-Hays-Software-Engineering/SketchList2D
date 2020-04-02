#ifndef PLACEABLE_H
#define PLACEABLE_H
#include "fx.h"
#include <FXRectangle.h>
class Placeable {
private:
	int xPos;
	int yPos;
	int height;
	int width;
	FXRectangle* rectangle;

public:
	Placeable();
	Placeable(int x, int y, int h, int w);
	FXRectangle* get_rectangle();
	void set_xPos(int newX);
	void set_yPos(int newY);
	int get_xPos();
	int get_yPos();

	void set_height(int newHeight);
	void set_width(int newWidth);
	int get_height();
	int get_width();
	~Placeable() {};
};

class Wall : public Placeable {
private:
	bool isInterior;

public:
	Wall(bool interior);
	bool get_isInterior();
};

#endif
