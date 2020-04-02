#ifndef PLACEABLE_H
#define PLACEABLE_H
#include "fx.h"
#include <FXRectangle.h>
class Placeable {
private:
	int xPos;
	int yPos;
	FXRectangle* rectangle;

public:
	Placeable();
	Placeable(int x, int y);
	FXRectangle* get_rectangle();
	void set_xPos(int newX);
	void set_yPos(int newY);
	int get_xPos();
	int get_yPos();
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
