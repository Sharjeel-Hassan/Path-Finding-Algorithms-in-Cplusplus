# Path-Finding-Algorithms-in-Cplusplus
Path finding from start to goal state using blind search algorithms.

Designed a robot navigation system which is able to conduct blind searches to find its path from start to goal state. As input, the system will take a description of the maze stored as a text file. The maze is a 2D grid with obstacles inside it. The obstacles are filled rectangles of unknown dimensions and can be found anywhere in the maze. The robot cannot be in those cells. There are 3 actions allowed.  Up one cell (cost is 2), right one cell (cost is 2), diagonally up towards the right(cost is 3). The system outputs:
1.	The complete path if goal is found, otherwise a failure message
2.	The total cost of the path
3.	A grid which shows the path followed.

FORMAT OF INPUT FILE (grid.txt)
Rows Cols //dimensions of the grid on line one 
start coordinates on line two 
goal coordinates on line three
the grid itself, one row per line.
There will be a zero for empty slot and one for an obstacle. The (0,0) coordinate is the bottom left cell.

ALGORITHMS TO IMPLEMENTED
1.	Breadth-first search
2.	Depth-first search
3.	Iterative deepening search
