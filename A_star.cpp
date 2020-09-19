#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

using namespace std;

typedef pair<int, int> Pair;


struct Node
{
	int h;
	int g;
	int f;

	Pair position;

	Node *parent;
};


// class Node
// {

// public:

// 	int h;
// 	int g;
// 	int f;

// 	Pair position;

// 	Node *parent;

// 	bool operator== (const Node &node);
// };


// bool Node::operator== (const Node &node)
// {
//     return (node.position.first  == this->position.first \
//     	&& 	node.position.second == this->position.second);
// }



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
										//  DIR		   COST

	int move[8][2] = {	{1 ,	0}, 	//  north 		10
						{-1,	0}, 	//  south 		10
						{0,		1}, 	//  east 		10
						{0,		-1}, 	//  west 		10
						{1, 	1}, 	//  north_east 	14
						{-1,	1}, 	//  south_east 	14
						{1,		-1},	//  north_west 	14
						{-1, 	-1} 	//  south_west 	14
					};
public:


	AStar(int x_0, int y_0, int x_f, int y_f, int density, int x, int y);
	~AStar();

	void 	printGrid();
	void 	BeginPathSearch();
	void 	tracePath(Node * node);

	bool 	isNotMovable(Pair pos);
	bool 	isOnList(Node node, vector<Node> list);

	int 	costH(Pair pos);
	int 	costG(Node node, Pair pos);
	void 	costF(Node new_node, Node curr_node);
};


AStar::AStar(int x_0, int y_0, int x_f, int y_f, int density = 2, int x = 10, int y = 10)
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
					grid[i][j] =  'o';
				else
					grid[i][j] =  '.';
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

int AStar::costG(Node node, Pair pos)
{
	if ( 	abs(node.position.first  - pos.first) 	== 1 &&\
			abs(node.position.second - pos.second) 	== 1)
	{
		return 14;
	}
	else
		return 10;
}

int AStar::costH(Pair pos)
{
	return abs(pos.first - x_f) + abs(pos.second - y_f);
}

bool AStar::isOnList(Node node, vector<Node> list)
{
	int i;

	for (i = 0; i < list.size(); i++)
	{
		if (node.position.first 	== list[i].position.first &&
			node.position.second	== list[i].position.second)
			return true;
	}
	return false;
}


void AStar::tracePath(Node *node)
{	

	Node *p = node;
	while(p != nullptr)
	{
		cout << "(" << p->position.first << "," << p->position.second << ")" << endl;
		p 	=	p->parent;
	}
}
void AStar::costF(Node new_node, Node curr_node)
{
	new_node.h = costH(new_node.position);
	new_node.g = costG(new_node, curr_node.position);
	new_node.f = new_node.h + new_node.g;
}

void AStar::BeginPathSearch()
{
	int 			i;
	int 		pos_x;
	int 		pos_y;
	int 		index;
	int  	 curr_pos;

	Node   start_node;
	Node 	curr_node;
	Node 	 end_node;

	Node 	*new_node;

	vector<Node> 	open_list;
	vector<Node>  closed_list;

	vector<Pair> path;
	//Initialize start and end nodes

	start_node.position.first 	= x_0;
	start_node.position.second 	= y_0;

	end_node.position.first 	= x_f;
	end_node.position.second 	= y_f;

	start_node.g 				= 	0;
	start_node.h 				= 	0;
	start_node.f 				= 	0;

	end_node.g 					= 	0;
	end_node.h 					= 	0;
	end_node.f 					= 	0;

	start_node.parent			= nullptr;
	end_node.parent				= nullptr;

	open_list.push_back(start_node);

	while(!open_list.empty())
	{

		index 		= 0;
		curr_node 	= open_list[0];
		path.push_back(open_list.back().position);
		for (i = 0; i < open_list.size(); i++) 	
		{	
			if (open_list[i].f < curr_node.f)
			{	
				index 			= 	i;
				curr_node	=	open_list[i];
			}
		}

		open_list.erase(open_list.begin() + index);
		closed_list.push_back(curr_node);

		for (curr_pos = 0; curr_pos < 8; curr_pos++)
		{
			pos_x = curr_node.position.first  + move[curr_pos][0];
			pos_y = curr_node.position.second + move[curr_pos][1];

			if (pos_x < 0 || pos_x > x - 1 || pos_y < 0 || pos_y > y - 1)
				continue;
			else if (grid[pos_x][pos_y] == 'o')
				continue;

			new_node 					= new Node; //create a new node
			new_node->position.first 	= pos_x; 	//assign position x
			new_node->position.second 	= pos_y;	//assign position y

			if (isOnList(*new_node, closed_list))
				continue;
			else if(!isOnList(*new_node, open_list))
			{	
				new_node->parent = &curr_node;
				costF(*new_node, curr_node);
				open_list.push_back(*new_node);
			}
			else
			{
				if (new_node->g < curr_node.g)
				{
					new_node->parent 	= &curr_node;
					new_node->g 	 	= costG(*new_node, curr_node.position);
					new_node->f 		= new_node->g + new_node->h;
				}
			}
		}

		if (isOnList(end_node, closed_list))
		{
			cout << "Solution is found !" << endl;
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
	// path_finding.printGrid();

	return(0);
}

// // void doNothing(Node node)
// // {
// // 	node.parent = nullptr;
// // }

// // int main()
// // {
// // 	struct Node new_node;

// // 	new_node.parent = nullptr;

// // 	doNothing(new_node);
// // 	return (0);
// // }