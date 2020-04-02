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

void Project::get_saveData(FXStream& stream)
{
	
	// Save my stuff to a stream
	numdata = 1;                 // number of total data values being serialized
	stream << numdata;           // Save the number of data values
	stream << gridSize;
}

void Project::loadProject(FXStream& stream)
{

	// Load my stuff from a stream
								 // number of total data values being serialized
	stream >> numdata;           // Save the number of data values
	stream >> gridSize;
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



