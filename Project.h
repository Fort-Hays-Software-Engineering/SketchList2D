#ifndef PROJECT_H
#define PROJECT_H
#include "fx.h"
#include <FXRectangle.h>
#include "Placeable.h"


class Project {
private:
	int gridSize;
	FXuint data[100], numdata;    // int variables for serialization of save files
	Placeable* placeables[100];
	int placeableCount;
public:

	Project();
	void set_gridSize(int size);
	int get_gridSize();
	void get_saveData(FXStream& stream); // get the char array to save to file
	void addPlaceable(int x, int y);



	~Project();
};

//as these get bigger split into new file

#endif //PROJECT_H