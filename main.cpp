#include "coordinate.h"
#include <iostream> 
#include <ctime> //random number tools
#include <cstdlib> //tokenizers and data type conversions
#include <cmath>
using namespace std; //to ditch std:: prefixes
void add_coordinate(Coordinate *list_end, float x, float y) //returns nothing, takes pointer to last node and the new x/y pair to be added. 
{
	if(list_end == nullptr)
	{
		cout << "list_end null, nothing to add.\n";
		return; //don't need to return anything to a void function. 
	}
	//end of list is passed in, so nodes need to be counted, summed, and incremented by 1 to find new ID. 
	int count = 0; //counter var
	for(Coordinate *moveto = list_end; moveto != nullptr; moveto = moveto->previous) //*moveto = list_end creates a pointer to the last node. while moveto points at a node AKA not a nullptr, it continues to move backwards using field previous.
	{
		count++; //end result plus 1 determines new ID. 
	}

	Coordinate *node = new Coordinate; //making room for the new coordinate pair
	node->x = x; //x val
	node->y = y; //y val
	node->coord_id = count + 1;  
	node->next = nullptr; //new final node's NEXT field is a nullptr. 
	node->previous = list_end; //the old list_end node is now the node just before the new one. 
	list_end->next = node; //previous last node's NEXT field is now the new node. actual list_end is updated in line 181, each time a new node is add3ed. 
}
//loops through and prints the list fwd
void forward_display(Coordinate *list_beginning)
{
	cout << "Default list:\n";
	for(Coordinate *moveto = list_beginning; moveto != nullptr; moveto = moveto->next)
	{
		cout << "ID " << moveto->coord_id << " Position: " << moveto->x << ", " << moveto->y << "\n";
	}
}
//prints the list in reverse
void backward_display(Coordinate *list_end)
{
	cout << "Backwards list:\n";
	for(Coordinate *moveto = list_end; moveto != nullptr; moveto = moveto->previous)
	{
		cout << "ID " << moveto->coord_id << " Position: " << moveto->x << ", " << moveto->y << "\n";
	}
}
//node deletion time
void delete_coordinate(Coordinate *list_beginning, int coord_id_to_delete) //takes first node in list AND target ID to delete
{
	Coordinate *node = list_beginning;
	while(node != nullptr && node->coord_id != coord_id_to_delete) //node address will never be nullpointer until the end of the list. it will count until coord id reaches target id. 
	{
		node = node->next;
	}

	if(node == nullptr)
	{
		cout << "ID not found; no data deleted.\n";
		return;
	}
	//edge case for first node only: first node was never passed by reference. it was passed by value. 
	if(node == list_beginning)
	{
		Coordinate *second = list_beginning->next;
		if(second == nullptr) //second would only be null if list contained 1 node
		{
			cout << "List contains 1 node; no data deleted.\n";
			return;
		}
		list_beginning->x = second->x; //node 1 takes node 2's data, and is set to point at node 3 instead. 
		list_beginning->y = second->y; //
		list_beginning->coord_id = second->coord_id; //
		list_beginning->next = second->next; //node 2 effectively bypassed here; the chain links node 1 and node 3. 
		if(second->next != nullptr)
		{
			second->next->previous = list_beginning; //second next field is node 3's address, node 3's previous is then set to node 1 instead of 2. 
		}
		delete second; //node 2's memory freed.

		return;
	}
	//default operation: remove connections from node to be deleted and connect neighbors.
	node->previous->next = node->next; //again, links backwards and then skips the middleman, node, and links to the entry AFTER node. link in the chain is bypassed, and the chain is repaired. 
	if(node->next != nullptr)
	{
		node->next->previous = node->previous;
	}
	delete node; //free up the bypassed node after hijacking its data. 
}
//counting entries
int list_length(Coordinate *list_beginning)
{
	int size = 0; //size or length, whatever
	for(Coordinate *moveto = list_beginning; moveto != nullptr; moveto = moveto->next) //move to first address, as long as move to <address> isn't null, change move to <address> to the next address. it's just counting. 
	{
		size++;
	}
	return size; //return result
}
//closest node search
void closest_to(Coordinate *list_beginning, float x, float y)
{
	if(list_beginning == nullptr)
	{
		cout << "List is empty. No nodes to compare.\n";
		return;
	}
	Coordinate *closest = list_beginning; //set pointer to first address. 
	float closest_node = sqrt(pow(closest->x - x, 2) + pow(closest->y - y, 2)); //distance formula essentially. 

	for(Coordinate *moveto = list_beginning->next; moveto != nullptr; moveto = moveto->next) //same increment loop
	{
		float deltax = moveto->x - x; //change in x
		float deltay = moveto->y - y; //change in y
		float dist = sqrt(deltax * deltax + deltay * deltay);
		if(dist < closest_node) // if lower than current min, becomes new min. 
		{
			closest_node = dist;
			closest = moveto;
		}
	}

	cout << "Closest coord pair to " << x << ", " << y << " is: " << closest->x << ", " << closest->y << " ID: " << closest->coord_id << "\n";
}
//main program
static float random_value() //we used rand in ECE131 to obtain random numbers. here, rand returns a number which is divided by rand_max for scaling. multiplying by 200 and then subtracting forces the range from 0-200 to -100 -> +100. STATIC just isolates the function within main.cpp. 
{
	return (float)rand() / RAND_MAX * 200.0f - 100.0f;
}
//move to the end and return the address. good for updating after deleting a node. 
static Coordinate *get_end(Coordinate *list_beginning) //static makes function unique to main.cpp. nothing else needs it. 
{
	Coordinate *moveto = list_beginning;
	while(moveto != nullptr && moveto->next != nullptr)
	{
		moveto = moveto->next;
	}
	return moveto; //return the address of the end, where the nullptr in the next field is found. 
}
//frees
static void free_list(Coordinate *list_beginning)
{
	while(list_beginning != nullptr)
	{
		Coordinate *next = list_beginning->next;
		delete list_beginning;
		list_beginning = next; //keep deleting and moving up the chain until memory is freed. 
	}
}
//main starts here
int main(int argc, char *argv[]) //same one we've used for the past 3 or 4 labs. argc looks for the right number of args, *argv[] is whatever string we input.
{
	if(argc != 2) //input rejection/error prints. 
	{
		cout << "Incorrect usage. Use:\n" << argv[0] << " <number of coordinates>\n";
		return 1;
	}

	int count = atoi(argv[1]); //convert command line string to integer
	if(count < 1)
	{
		cout << "Please choose an integer value greater than or equal to 1.\n";
		return 1;
	}

	srand(time(0)); //using runtime as rand seed. guarantees each consecutive run will use different values.
	 
	//building first node manually.
	Coordinate *list_beginning = new Coordinate;
	list_beginning->x = random_value();
	list_beginning->y = random_value();
	list_beginning->coord_id = 1;
	list_beginning->next = nullptr;
	list_beginning->previous = nullptr;
	Coordinate *list_end = list_beginning;

	//linking the rest
	for(int i = 1; i < count; i++) //start at 1 because 1 node exists already. 
	{
		add_coordinate(list_end, random_value(), random_value()); //pointer to list end, random x, random y
		list_end = list_end->next;
	}

	cout << "List size: " << list_length(list_beginning) << " entries.\n";
	forward_display(list_beginning);
	backward_display(list_end);
	
	//deletions
	int id_to_delete; //arbitrary id number selected to delete.
	if(count >= 2)
	{
		id_to_delete = 2;
	}
	else
	{
		id_to_delete = 1; //my program won't actually delete the only node. this checks whether the ID IS the only node. 
	}
	
	cout << "Deleting ID " << id_to_delete << "\n";
	delete_coordinate(list_beginning, id_to_delete);
	list_end = get_end(list_beginning);

	cout << "List size: " << list_length(list_beginning) << " entries.\n";
	forward_display(list_beginning);
	backward_display(list_end);

	//closest coord
	cout << "\n";
	closest_to(list_beginning->next, list_beginning->x, list_beginning->y); //we use the first node for a fixed arbitrary reference. could use anything. start at the 2nd node so the first note isn't compared to itself. 

	free_list(list_beginning); //starting at the very first node, free it all. 
	return 0;

}
