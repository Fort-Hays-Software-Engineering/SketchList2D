#ifndef PLACEABLE_H
#define PLACEABLE_H
#include "fx.h"
#include <FXRectangle.h>
class Placeable : public FXObject{
private:
	int xPos;
	int yPos;
	int height;
	int width;
	int angle;
	FXRectangle* rectangle;

public:
	Placeable();
	Placeable(int x, int y, int h, int w);
	FXRectangle* get_rectangle();
	void set_xPos(int newX);
	void set_yPos(int newY);
	int get_xPos();
	int get_yPos();
	int get_angle();
	void set_angle(int newAngle);

	void set_height(int newHeight);
	void set_width(int newWidth);
	int get_height();
	int get_width();

	void draw(FXDCWindow* dc);

	FXDataTarget	   heightTarget;			// Data target for height box
	FXDataTarget	   widthTarget;			    // Data target for width box

	void save(FXStream& stream); // Serialize placeable data for save file

	~Placeable() {};
};

class Wall : public Placeable {
private:
	FXbool isInterior;

public:
	Wall(bool interior);
	bool get_isInterior();
};

#endif
