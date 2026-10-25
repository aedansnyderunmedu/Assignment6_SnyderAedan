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
		return; //don't need to return anything to a void function. 
	}
	//end of list is passed in, so nodes need to be counted, summed, and incremented by 1 to find new ID. 
	int count = 0; //counter var
	for(Coordinate *moveto = list_end; moveto != nullptr; moveto->previous) //*moveto = list_end creates a pointer to the last node. while moveto points at a node AKA not a nullptr, it continues to move backwards using field previous.
	{
		count++; //end result plus 1 determines new ID. 
	}

	Coordinate *node = new Coordinate; //making room for the new node aka coordinate pair
	node->x = x; //x val
	node->y = y; //y val
	node->coord_id = length + 1;  
	node->next = nullptr; //new final node
	node->previous = list_end; //previous final node set as our initial start. each coord basically contains a two-sided record/position. pretty cool honestly. 
	list_end->next = node; //
}
//loops through and prints the list fwd
void forward_display(Coordinate *list_beginning)
{
	cout << "Default list:\n";
	for(Coordinate *moveto = list_beginning; moveto != nullptr; moveto = moveto->next)
	{
		cout << "ID " << moveto->coord_id << "Position: " << moveto->x << ", " << moveto->y << "\n";
	}
}
//prints the list in reverse
void backward_display(Coordinate *list_end)
{
	cout << "Backwards list:\n";
	for(Coordinate *moveto = list_end; moveto != nullptr; moveto->previous)
	{
		cout << "ID " << moveto->coord_id << "Position: " << moveto->x << ", " << moveto->y << "\n";
	}
}
//node deletion time
void delete_coordinate(Coordinate *list_beginning; int coord_id_to_delete)
{
	Coordinate *node = list_beginning;
	while(node != nullptr && node->coord_id != coord_id_to_delete)
	{
		node = node->next;
	}

	if(node == nullptr)
	{
		cout << "ID not found; no data deleted.\n";
		return;
	}
	//for first node only: first node was never passed by reference. it was passed by value. 
	if(node == list_beginning)
	{
		Coordinate *second = list_beginning->next;
		if(second == nullptr)
		{
			cout << "List contains 1 node; no data deleted.\n";
			return;
		}
		list_beginning->x = second->x;
		list_beginning->y = second->y;
		list_beginning->coord_id = second->coord_id;
		list_beginning->next = second->next;
		if(second->next != nullptr)
		{
			second->next->previous = list_beginning;
		}
		delete second;
		return;
	}
	//default operation: remove connections from node to be deleted and connect neighbors.
	node->previous->next = node->next;
	if(node->next != nullptr)
	{
		node->next->previous = node->previous;
	}
	delete node;
}
//counting entries
int list_length(Coordinate *list_beginning)
{
	int size = 0;
	for(Coordinate *moveto = list_beginning; moveto != nullptr; moveto = moveto->next)
	{
		size++;
	}
	return size;
}
//closest node search
void closest_to(Coordinate *list_beginning, float x, float y)
{
	if(list_beginning == nullptr)
	{
		cout << "List is empty. No nodes to compare.\n";
		return;
	}
	Coordinate *closest = list_beginning;
	float closest_node = sqrt(pow(closest->x - x, 2) + pow(closest->y - y, 2));

	for(Coordinate *moveto = list_beginning->next; moveto != nullptr; moveto = moveto->next)
	{
		float deltax = moveto->x - x;
		float deltay = moveto->y - y;
		float dist = sqrt(deltax * deltax + deltay * deltay);
		if(dist < closest_node)
		{
			closest_node = dist;
			closest = moveto;
		}
	}

	cout << "Closest node to " << x << ", " << y << "is: " << closest->x << ", " << closest->y << "ID " << closest->coord_id;
}


