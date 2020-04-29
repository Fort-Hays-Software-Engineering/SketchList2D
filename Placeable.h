#ifndef PLACEABLE_H
#define PLACEABLE_H
#include "fx.h"
#include <FXRectangle.h>
#define PI 3.14159265358979323846
class Placeable : public FXObject{
private:
	int xPos;						// X position Coordinate 
	int yPos;						// Y position Coordinate
	int height;						// Height variable
	int width;						// Width variable
	int v;							// Version variable, used to determine save file type
	int angle;						// Angle variable
	int s6;							// placeholder variable for another specification
	int s7;							// placeholder variable for another specification
	FXRectangle* rectangle;         // The unrotated rectangle for the placeale
	FXPoint p[4];                   // the points of the rotated rectangle

	void updatePoints();

public:
	Placeable();
	Placeable(int x, int y, int h, int w);
	Placeable(int x, int y, int h, int w, int a);
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
	bool isClicked(int clickX, int clickY);

	void draw(FXDCWindow* dc);
	void drawControlHandles(FXDCWindow* dc);

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
