#ifndef PROJECT_H
#define PROJECT_H
#include "fx.h"

class Project {
private:
	int gridSize;
	FXuint data[100], numdata;    // int variables for serialization of save files
public:

	Project();
	void set_gridSize(int size);
	int get_gridSize();
	void get_saveData(FXStream& stream); // get the char array to save to file

	~Project();
};

//as these get bigger split into new file
class Placeable {
private:
	int xPos;
	int yPos;

public:
	Placeable(int x, int y);
	void set_xPos(int newX);
	void set_yPos(int newY);
	int get_xPos();
	int get_yPos();
	~Placeable();
};

class Wall : public Placeable {
private:
	bool isInterior;

public:
	Wall(bool interior);
	bool get_isInterior();
};
#endif //PROJECT_H