#include "coordinate.h"
#include <iostream> 
#include <ctime> //random number tools
#include <cstdlib> //tokenizers and data type conversions
#include <math>
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
//main program
static float random_value() //we used rand in ECE131 to obtain random numbers. here, rand returns a number which is divided by rand_max for scaling. multiplying by 200 and then subtracting forces the range from 0-200 to -100 -> +100. STATIC just isolates the function within main.cpp. 
{
	return (float)rand() / RAND_MAX * 200.0f - 100.0f;
}
//move to the end and return the count. good for updating after deleting a node. 
static Coordinate *get_end(Coordinate *list_beginning)
{
	Coordinate *moveto = list_beginning;
	while(moveto != nullptr && moveto->next != nullptr)
	{
		moveto = moveto->next;
	}
	return moveto;
}
//frees
static void free_list(Coordinate *list_beginning)
{
	while(list_beginning != nullptr)
	{
		Coordinate *next = list_beginning->next;
		delete list_beginning;
		list_beginning = next;
	}
}
//main starts here
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cout << "Incorrect usage. Use:\n" << argv[0] << "<number of coordinates>\n";
		return 1;
	}

	int count = atoi(argv[1]); //convert command line string to integer
	if(count < 1)
	{
		cout << "Please choose an integer value greater than or equal to 1.\n";
		return 1;
	}

	srand(time(0)); //using runtime as rand seed. guarantees each consecutive run will use different values.
	 
	//building first seed manually.
	Coordinate *list_beginning = new Coordinate;
	list_beginning->x = random_value();
	list_beginning->y = random_value();
	list_beginning->coord_id = 1;
	list_beginning->next = nullptr;
	list_beginning->previous = nullptr;
	Coordinate *list_end = list_beginning;

	//linking the rest
	for(int i = 1; i < count; i++)
	{
		add_coordinate(list_end, random_value(), random_value());
		list_end = list_end->next;
	}

	cout << "List size: " << list_length(list_beginning) << " entries./n";
	forward_display(list_beginning);
	backward_display(list_end);
	
	//deletions
	int id_to_delete = (count >= 2) ? 2 : 1;
	cout << " Deleting ID " << id_to_delete << "\n";
	delete_coordinate(list_beginning, id_to_delete);
	list_end = find_end(list_beginning);

	cout << "List size: " << list_length(list_beginning) << " entries.\n";
	forward_display(list_beginning);
	backward_display(list_end);

	//closest coord - WIP
	cout << "\n";

	free_list(list_beginning);
	return 0;

}
