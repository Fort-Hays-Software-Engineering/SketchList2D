#include "Project.h"
#include "fx.h"
Project::Project()
{
	gridSize = 16; //one inch (16 1/16ths of an inch)
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



