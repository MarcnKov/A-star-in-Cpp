# A*-Pathfinding-in-C++
A basic pathfinding A* algorithm implementation on a 2D grid.

## Credits

This algorithm was implemented following Patrick's Lesters article ["A* Pathfinding for Beginners"](https://github.com/averagemarcin/A-star-in-Cpp/blob/master/Astar.pdf). 
Patrick provides a clear explanation on the workings of the algorithm. I highly recommend that you start 
by reading his article if you want to understand and implement A*. 

## Description

The goal of the algorithm is to find the shortest path in a graph between the 
starting and final nodes. In order to do find the path, A* relies on a heuristic (Manhattan distance)  
that allows it to estimate the shortest distance to the final node. 

The A* algorithm finds the path in a 2D grid. 2D grid is composed of movable and not movable 
areas. Walking path is represented by *.* and obstacle is represented 
by *o*, start and ending nodes are represented by * and *x* accordingly.

## Future Improvements

While algorithm finds the path it's not able to trace the path back from the
final to the starting node.

## Usage

Download the A_star.cpp file to your working directory and compile it.
Then run the executable file providing the necessary arguments :
(example on my Linux OS, executable is named a.out)

*/a.out start_pos_x start_pos_y end_pos_x end_pos_y density grid_size_x grid_size_y*
*./a.out 10 15 25 30 7 40 40*

## Input -> Output

[Example input -> output](https://github.com/averagemarcin/A-star-in-Cpp/blob/master/a_star.png)
