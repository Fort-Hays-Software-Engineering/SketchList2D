#include "Project.h"
#include "Placeable.h"
#include "fx.h"


Project::Project()
{
	gridSize = 16; //one inch (16 1/16ths of an inch)
	placeableCount = 0;
}

void Project::set_gridSize(int size)
{
	gridSize = size;
	for (int i = 0; i < placeableCount; i++) {
		placeables[i]->set_curgrid(size);
	}

}

int Project::get_gridSize()
{
	return gridSize;
}

void Project::save(FXStream& stream)
{


	// Save my stuff to a stream

	stream << gridSize;             // Save the number of data values
	stream << placeableCount;		// save number of placeables

	// Serialize and Save placeables
	for (int i = 0; i < placeableCount; i++) {
		placeables[i]->save(stream);
	}
}

void Project::load(FXStream& stream)
{
	FXint x, y, h, w, a, v, prefabType;
	FXbool isPrefab;
	FXString name;
	// Load Project from stream
	
	stream >> gridSize;
	stream >> placeableCount;

	// load placeables
	for (int i = 0; i < placeableCount; i++) {
		
		stream >> x >> y >> h >> w >> v >> a >> isPrefab >> prefabType >> name;
		// If version variable is null, the project was saved with an angle
		if (v == NULL) {
			if (isPrefab == 1) {
				placeables[i] = new Placeable(true, prefabType, x, y, 0);
				placeables[i]->set_curgrid(gridSize);
				placeables[i]->set_width(w);
				placeables[i]->set_height(h);
				placeables[i]->set_angle(a);
			}
			else {
				placeables[i] = new Placeable(x, y, h, w, a, gridSize, name);
			}
			
			
		}

		x = y = h = w = v = a = isPrefab = prefabType = NULL;
		
	}
}

void Project::addPlaceable(int x, int y, int h, int w)
{
	placeables[placeableCount] = new Placeable(x, y, h, w, 0, x);
	placeableCount++;
}

void Project::addPlaceable(bool, int type, int x, int y)
{
	placeables[placeableCount] = new Placeable(true, type, x, y, 0);
	placeableCount++;
}

int Project::get_placeableCount()
{
	return placeableCount;
}



