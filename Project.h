#ifndef PROJECT_H
#define PROJECT_H
#include "fx.h"
#include <FXRectangle.h>
#include "Placeable.h"


class Project{
private:
	FXuint gridSize;
	FXuint data[100], numdata;    // int variables for serialization of save files
	
	int placeableCount;
public:
	Placeable* placeables[100];
	Project();
	void set_gridSize(int size);
	int get_gridSize();
//	void loadProject(FXStream& stream); // deserialize project data from save file
//	void get_saveData(FXStream& stream); // Serialize project data for save file
	void addPlaceable(int x, int y, int h, int w);
	int get_placeableCount();



	//~Project();
};

//as these get bigger split into new file

#endif //PROJECT_H