#include "coordinate.h"
#include <iostream> 
#include <random> //random number tools
#include <cstdlib> //tokenizers and data type conversions
using namespace std; //to ditch std:: prefixes
void add_coordinate(Coordinate *list_end, float x, float y)
{
	if(list_end == nullptr)
	{
		cout << "list_end null, nothing to add.\n";
		return;
	}
	//end of list is passed in, so nodes need to be counted, summed, and incremented by 1 to find new ID. 
	int count = 0; //counter var
	for(Coordinate *goto = list_end; goto != nullptr; goto->previous)
	{
		count++;
	}

	Coordinate *node = new Coordinate; //making room for the new node aka coordinate pair
	node->x = x; //x val
	node->y = y; //y val
	node->coord_id = length + 1;  
	node->next = nullptr;
