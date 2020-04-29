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
	FXint x, y, h, w;
	// Load my stuff from a stream
	
	stream >> gridSize;
	stream >> placeableCount;

	// load placeables
	for (int i = 0; i < placeableCount; i++) {
		
		stream >> x >> y >> h >> w;
		placeables[i] = new Placeable(x, y, h, w);
		x = y = h = w = NULL;
	}
}

void Project::addPlaceable(int x, int y, int h, int w)
{
	placeables[placeableCount] = new Placeable(x, y, h, w);
	placeableCount++;
}

int Project::get_placeableCount()
{
	return placeableCount;
}



