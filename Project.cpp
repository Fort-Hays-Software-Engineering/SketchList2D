#include "Project.h"

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
