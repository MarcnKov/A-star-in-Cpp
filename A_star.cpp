#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>

using namespace std;

typedef pair<int, int> Pair;

class Node
{

public:

	int h;
	int g;
	int f;

	Pair   parent;
	Pair position;
};


class AStar
{

private:

	int density; //obstacle density

	int x; 		 //size of the grid  row
	int y; 		 //size of the grid  column

	int x_0; 	 //initial 	position row
	int y_0;	 //initial 	position column

	int x_f;	 //final 	position row
	int y_f;	 //final 	position column

	char **grid;

public:


	AStar(int x_0, int y_0, int x_f, int y_f, int density, int x, int y);
	~AStar();

	void printGrid();
	void aStarSearch();
	//implement returnPath function

	int 	distance( int x, int y);

	bool 	positionIsValid(Pair position);
	bool 	isNotBlocked(int x, int y);
	bool 	visitedChild(Node children, vector<Node> list, bool flag);
};


AStar::AStar(int x_0, int y_0, int x_f, int y_f, int density = 2, int x = 20, int y = 20)
	{
		int i;
		int j;

		this->x 		= 	x;
		this->y 		= 	y;

		this->x_0 		= x_0;
		this->y_0 		= y_0;

		this->x_f 		= x_f;
		this->y_f 		= y_f;

		this->density 	= density;

		grid 			= new char*[x];

		srand (time(NULL));
		for (i = 0; i < y; i++)
		{
			grid[i] = new char[y];
			for (j 	= 0; j < x; j++)
			{
				if (int (rand() % y) < density)
				{
					grid[i][j] =  'o';
				}
				else
				{
					grid[i][j] =  '.';
				}
			}
		}

		grid[x_0][y_0]  = '*'; //start
		grid[x_f][y_f] 	= 'x'; //finish

	}

AStar::~AStar()
	{
		delete *grid;
	}


void AStar::printGrid()
{
	int i;
	int j;

	for (i = 0; i < y; i++)
	{
		for (j = 0; j < x; j++)
		{
			cout << grid[i][j];
		}
		cout << endl;
	}
}

int AStar::distance(Pair pos)
{
	return abs(pos.first - x_f) + abs(pos.second - x_f);
}

bool AStar::isNotBlocked(int x, int y)
{
	if (grid[x][y] != 'o')
	{
		return true;
	}
	else
	{
		return false;
	}		
}

bool AStar::positionIsValid(Pair position)
{
	if (	position.first 	> x - 1 || position.first 	< 0\
		|| 	position.second > y - 1 || position.second 	< 0)
	{
		return false;
	}
	else if(grid[position.first][position.second] == 'o')
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool AStar::visitedChild(Node children, vector<Node> list, bool flag = false)
{
	int i;

	for (i = 0; i < list.size(); i++)
	{
		if (flag == false)
		{
			if (children.position == list[i].position)
			{
				return true;
			}
		}
		else
		{
			if (children.position == list[i].position && children.g > list[i].g)
			{
				return true;
			}
		}
	}
	return false;
}

void AStar::aStarSearch()
{
	//Declare utility variables
	int current_index;
	int 	direction;
	int 	 position;
	int 		child;
	int 		 cost;
	int 			i;

	Pair node_position;

	//Declare nodes
	Node start_node;
	Node final_node;
	Node *new_node;
	Node current_node;


	//Declare to_visit and visited vectors

	vector<Node>   to_visit_list;
	vector<Node> 	visited_list;
	vector<Node> 		children;

	//Declare and initialize movemnt array
	direction = 4;
	int move[direction][2] = {	{-1, 0},  //go up
								{0 ,-1},  //go left
								{1 , 0},  //go down
								{0 , 1} };//go right

	//Initialize start and end nodes

	start_node.parent.first 	= 	0;
	start_node.parent.second 	= 	0;

	start_node.position.first 	= x_0;
	start_node.position.second 	= y_0;

	start_node.g 				= 	0;
	start_node.h 				= 	0;
	start_node.f 				= 	0;

	final_node.parent.first 	= 	0;
	final_node.parent.second 	= 	0;

	final_node.position.first 	= x_f;
	final_node.position.second 	= y_f;

	start_node.g 				= 	0;
	start_node.h 				= 	0;
	start_node.f 				= 	0;

	to_visit_list.push_back(start_node);

	while (!to_visit_list.empty())
	{
		current_node 	= to_visit_list[0];
		current_index 	= 				 0;

		for (i = 0; i < to_visit_list.size(); i++)
		{
			if (to_visit_list[i].f < current_node.f)
			{
				current_node 	= to_visit_list[i];
				current_index 	= 				 i;
			}
		}

		to_visit_list.erase(to_visit_list.begin() + current_index);
		visited_list.push_back(current_node);

		if (current_node.position == final_node.position)
		{
			cout << "Solution is found!" << endl;
		}

		for (position = 0; position < direction; position++)
		{
			node_position.first 	= current_node.position.first 	+ move[position][0];
			node_position.second 	= current_node.position.second 	+ move[position][1];

			if(!positionIsValid(node_position))
			{
				continue;
			}

			new_node 			= new Node;
			new_node->parent 	= current_node.position;
			new_node->position 	= node_position;

			children.push_back(*new_node);
		}

		for (child = 0; child < children.size(); child++)
		{
			if (visitedChild(children[child], visited_list))
			{
				continue;
			}

			children[child].g = current_node.g + 1;
			children[child].h = distance(children[child].position);
			children[child].f = children[child].g + children[child].h;

			if (visitedChild(children[child], to_visit_list, true))
			{
				continue;
			}

			to_visit_list.push_back(children[child]);
		}

	}

}

int main(int argc, char* argv[])
{
	if (argc < 5)
	{
        cerr << "Usage: " << argv[0] << " x_0, y_0, x_f, y_f, density, x, y" << endl;
        return 1;
    }

	AStar path_finding(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));


	path_finding.printGrid();
	// path_finding.aStarSearch();


	return(0);
}