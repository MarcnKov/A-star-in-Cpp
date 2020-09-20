#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

using namespace std;

typedef pair<int, int> Pair;

class Node
{

public:

/*Cost variables */

int h;
int g;
int f;

/*(x,y) position of a node*/

Pair position;

/*Pointer to the parent of a gieven node.*/

Node *parent;

bool operator== (const Node &node);
};

bool Node::operator== (const Node &node)
{
    return (node.position.first  == this->position.first &&
	    node.position.second == this->position.second);
}

class AStar
{

private:	
	/*obstacle density*/
	int density;

	/*size of the grid row, col*/
	int 	  x;
	int 	  y;

	/*initial position row, col*/
	int 	x_0;
	int 	y_0;

	/*final position row, col*/
	int x_f;
	int y_f;

	/*Path grid*/
	char **grid;

	/*Initialize & declare a 2D moving array
	
	traversal direction | traversal cost
	north 			10
	south 			10
	east 			10
	west 			10
	north_east 		14
	south_east 		14
	north_west 		14
	south_west 		14 */
	
	int move[8][2] ={{1 ,	0}, 		
			{-1,	0}, 
			{0,	1}, 
			{0,	-1},
			{1, 	1},
			{-1,	1},
			{1,	-1},
			{-1, 	-1} };
	
	
public:
	AStar(int x_0, int y_0, int x_f, int y_f, int density, int x, int y);
	~AStar();
	
	void printGrid();
	void BeginPathSearch();
	void tracePath(Node *node);
	void costF(Node new_node, Node curr_node);

	bool isNotMovable(Pair pos);
	bool isOnList(Node node, vector<Node> list);

	int costH(Pair pos);
	int costG(Node node, Pair pos);
};

/* 	Constructor for AStar class. Initialize the variables
	and create the 2D grid (path) with pseudo-random
  	obstacles.						*/

AStar::AStar(int x_0, int y_0, int x_f, int y_f, int density = 2, int x = 10, int y = 10)
{
	int i;
	int j;

	this->x = x;
	this->y = y;

	this->x_0 = x_0;
	this->y_0 = y_0;

	this->x_f = x_f;
	this->y_f = y_f;

	this->density = density;

	grid = new char*[x];

	srand (time(NULL));
	for (i = 0; i < y; i++)
	{
		grid[i] = new char[y];
		for (j 	= 0; j < x; j++)
		{
			if (int (rand() % y) < density)
				grid[i][j] =  'o'; //obstacle
			else
				grid[i][j] =  '.'; //walkable path
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


/*
Calculate the cost from the current node
to the each of its surrounding 8 squares.
Count horizontal or vertical distance as
10 points.
Count diagonal distance as 14 points.
*/
int AStar::costG(Node node, Pair pos)
{
	if ( abs(node.position.first - pos.first) == 1 && abs(node.position.second - pos.second) == 1)
		return 14;
	else
		return 10;
}

/*
Calculate the distance from a given square to
the end square using the Manhattan method.
*/
int AStar::costH(Pair pos)
{
	return abs(pos.first - x_f) + abs(pos.second - y_f);
}

/*Check if a node is on a list */
bool AStar::isOnList(Node node, vector<Node> list)
{
	int i;

	for (i = 0; i < list.size(); i++)
	{
		if (node == list[i])
			return true;
	}
	return false;
}

/*Trace path from the end node.
	Isn't working! */

void AStar::tracePath(Node *node)
{	
	Node  *p = node;
	while(p != nullptr)
	{
		cout << "(" << p->position.first << "," << p->position.second << ")" << endl;
		p = p->parent;
	}
}

/*Calculate the total cost as the sum of two
costs. */
void AStar::costF(Node new_node, Node curr_node)
{
	new_node.h = costH(new_node.position);
	new_node.g = costG(new_node, curr_node.position);
	new_node.f = new_node.h + new_node.g;
}

void AStar::BeginPathSearch()
{

	int i;
	int pos_x;
	int pos_y;
	int index;
	int curr_pos;

	Node start_node;
	Node curr_node;
	Node end_node;

	Node *new_node;
	Node *tmp_node;

	vector<Node>	open_list;
	vector<Node>	closed_list;

	start_node.position.first  = x_0;
	start_node.position.second = y_0;

	end_node.position.first    = x_f;
	end_node.position.second   = y_f;

	start_node.g = 	0;
	start_node.h = 	0;
	start_node.f = 	0;

	end_node.g = 	0;
	end_node.h = 	0;
	end_node.f = 	0;

	start_node.parent = nullptr;
	end_node.parent	  = nullptr;

	/* Push the starting node on the open list. */
	open_list.push_back(start_node);
	
	while(!open_list.empty())
	{

		/* Look for the lowest F cost node on the
		open_list. Name it current node(squre).*/
		index 		= 0;
		curr_node 	= open_list[0];
		for (i = 0; i < open_list.size(); i++) 	
		{	
			if (open_list[i].f < curr_node.f)
			{	
				index 	  = i;
				curr_node = open_list[i];
			}
		}
		/*
		Remove the current node from the open_list,
		put it on the closed_list.
		*/
		open_list.erase(open_list.begin() + index);
		closed_list.push_back(curr_node);

		/*
		For each of the 8 squares adjacent
		to the current square repeat :
		*/
		for (curr_pos = 0; curr_pos < 8; curr_pos++)
		{
			/*
			Calculate the distance of a current square
			to each of the 8 adjacent squares,
			i.e. calculate positions on the (x,y) grid 
			*/
			pos_x = curr_node.position.first  + move[curr_pos][0];
			pos_y = curr_node.position.second + move[curr_pos][1];

			/* If it is not walkable ignore it. */
			if (pos_x < 0 || pos_x > x - 1 || pos_y < 0 || pos_y > y - 1)
				continue;
			else if (grid[pos_x][pos_y] == 'o')
				continue;
			/*
			Create a new node and assign its coordinates
			calculated in the previous step 
			*/
			new_node = new Node;
			new_node->position.first  = pos_x;
			new_node->position.second = pos_y;
			/* If it is on the closed list ignore it. */
			if (isOnList(*new_node, closed_list))
				continue;

			/* If it isn’t on the open list, add it  to
			the open list. Make the current square  the
			parent of this square. Record the F, G, and
			H costs of the square.
			*/
			else if(!isOnList(*new_node, open_list))
			{	
				new_node->parent = &curr_node;

				costF(*new_node, curr_node);
				open_list.push_back(*new_node);
			}

			/*If it is on the open list already, check
			to see if this path to that square is 
			better, using G cost as the measure. A 
			lower G cost means that this is a better 
			path. If so, change the parent of the square
			to the current square, and recalculate the G
			and F scores of the square.*/
			else
			{
				if (new_node->g < curr_node.g)
				{
					new_node->parent = &curr_node;
					new_node->g = costG(*new_node, curr_node.position);
					new_node->f = new_node->g + new_node->h;
				}
			}
		}

		/*Check if the end square is on the closed
		list, if it's the case the path has been
		found.*/
		if (isOnList(end_node, closed_list))
		{
			cout << "Path is found !" << endl;
			tracePath(&end_node);
			return;
		}
	}
	cout << "There is no path !" << endl;
	return;
}

int main(int argc, char* argv[])
{
	if (argc < 8)
	{
		cerr << "Usage: " << argv[0] << " x_0, y_0, x_f, y_f, density, x, y" << endl;
		return 1;
	}
	else if (	stoi(argv[1]) >= stoi(argv[6]) || stoi(argv[3]) >= stoi(argv[6]) ||
    			stoi(argv[2]) >= stoi(argv[7]) || stoi(argv[4]) >= stoi(argv[7]))
	{
		cerr << "Usage: \n(x_0 < x, x_f < x) \n(y_0 < y, y_f < y)" << endl;
		return 1;
	}

	AStar path_finding(	stoi(argv[1]), stoi(argv[2]),
				stoi(argv[3]), stoi(argv[4]),
				stoi(argv[5]), stoi(argv[6]),
				stoi(argv[7]) );

	path_finding.printGrid();
	path_finding.BeginPathSearch();


	return(0);
}