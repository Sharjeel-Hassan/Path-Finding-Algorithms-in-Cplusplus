#include <iostream>
#include <conio.h>
#include <fstream>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <windows.h>
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

using namespace std;

struct State{
	int x;
	int y;
	int cost;// cost will represent the cost of the path
	int parentx;
	int parenty;
};


//global variables
State initialstate;
State currentstate;
State goalstate;
int** grid;
bool** visited;
int row, col;
queue<State> myQueue;//queue to maintain BFS
stack<State> stackPath;// to maintaiin the path
stack<State> stackDFS;//for DFS
stack<State> stackLDFS;//for LimitedDFS

void inputFile(string name) {
	ifstream fin(name);
	fin >> row;
	fin >> col;
	grid = new int*[row];
	visited = new bool* [row];

	for (int i = 0;i < row;i++) {
		grid[i] = new int[col];
		visited[i] = new bool[col];
	}

	fin >> initialstate.x;
	fin >> initialstate.y;

	fin >> goalstate.x;
	fin >> goalstate.y;

	for (int i = 0;i < row;i++) {
		for (int j = 0;j < col;j++) {
			fin >> grid[i][j];
			visited[i][j] = false;
		}
	}
	
	currentstate = initialstate;
	currentstate.cost = 0;

	grid[initialstate.x][initialstate.y] = 2;//representing 2 as initial state
	grid[goalstate.x][goalstate.y] = 3;//representing 3 as goal state
}

void printGrid() {
	SetConsoleTextAttribute(out, 7);
	cout << "Stat: (" << initialstate.x << "," << initialstate.y << ")"<<endl;
	cout << "End: (" << goalstate.x << "," << goalstate.y << ")" << endl;
	
	for (int i = row - 1;i >= 0;i--) {
		for (int j = 0;j < col;j++)
			if (grid[i][j] == 2) {
				SetConsoleTextAttribute(out, 192);
				cout << "S ";
				SetConsoleTextAttribute(out, 7);
			}
			else if (grid[i][j] == 3) {
				SetConsoleTextAttribute(out, 192);
				cout << "G ";
				SetConsoleTextAttribute(out, 7);
			}
			else if (grid[i][j] == 4) {//to print the path
				SetConsoleTextAttribute(out, 160);
				cout << "* ";
				SetConsoleTextAttribute(out, 7);
			}
			else {
				if (grid[i][j] == 1) {
					SetConsoleTextAttribute(out, 128);
				}
				else {
					SetConsoleTextAttribute(out, 112);
				}
				cout << grid[i][j] << " ";
				SetConsoleTextAttribute(out, 7);
			}
		cout << endl;
	}
}

bool compare(State arg1, State arg2) {
	if (arg1.x == arg2.x && arg1.y == arg2.y)
		return true;
	else
		return false;
}

void successorFunctions(State obj,int type) {
	State temp;
	if (obj.y + 1 < col && visited[obj.x][obj.y+1] == false && grid[obj.x][obj.y + 1]!=1) {
		temp.y = obj.y + 1;
		temp.x = temp.parentx = obj.x;
		temp.cost = obj.cost + 2;
		temp.parenty = obj.y;
		if (type == 0)//BFS
			myQueue.push(temp);
		else if (type == 1)//DFS
			stackDFS.push(temp);
		else if (type == 2)//LDFS
			stackLDFS.push(temp);
	}
	if (obj.x + 1 < row && obj.y + 1 < col && visited[obj.x + 1][obj.y+1] == false && grid[obj.x + 1][obj.y+1] != 1) {
		temp.x = obj.x + 1;
		temp.y = obj.y + 1;
		temp.parenty = obj.y;
		temp.cost = obj.cost + 3;
		temp.parentx = obj.x;
		if (type == 0)//BFS
			myQueue.push(temp);
		else if (type == 1)//DFS
			stackDFS.push(temp);
		else if (type == 2)//LDFS
			stackLDFS.push(temp);
	}
	if (obj.x + 1 < row && visited[obj.x+1][obj.y] == false && grid[obj.x + 1][obj.y] != 1) {
		temp.x = obj.x + 1;
		temp.y = temp.parenty = obj.y;
		temp.cost = obj.cost + 2;
		temp.parentx = obj.x;
		if (type == 0)//BFS
			myQueue.push(temp);
		else if (type == 1)//DFS
			stackDFS.push(temp);
		else if (type == 2)//LDFS
			stackLDFS.push(temp);
	}
}

bool BFS(){
	myQueue.push(currentstate);
	State temp;
	bool ans = false;
	while (!myQueue.empty())
	{
		temp=myQueue.front();
		if (visited[temp.x][temp.y] == false) {
			visited[temp.x][temp.y] = true;
			stackPath.push(temp);
			if (compare(temp, goalstate)) {
				ans = true;
				break;
			}
		successorFunctions(temp,0);
		}
		myQueue.pop();
	}
	return ans;
}

bool DFS() {
	stackDFS.push(currentstate);
	State temp;
	bool ans = false;
	while (!stackDFS.empty())
	{
		temp = stackDFS.top();
		stackDFS.pop();
		if (visited[temp.x][temp.y] == false) {
			visited[temp.x][temp.y] = true;
			stackPath.push(temp);
			if (compare(temp, goalstate)) {
				ans = true;
				break;
			}
			successorFunctions(temp, 1);
		}
	}
	return ans;
}

bool LimitedDFS(int limit) {			//utility function, to be used in IDS
	stackLDFS.push(currentstate);
	State temp;
	bool ans = false;
	while (!stackLDFS.empty())
	{
		temp = stackLDFS.top();
		if (visited[temp.x][temp.y] == false) {
			visited[temp.x][temp.y] = true;
			stackPath.push(temp);
			if (compare(temp, goalstate)) {
				ans = true;
				break;
			}
			if (stackLDFS.size() <= limit) {
				successorFunctions(temp, 2);
			}
		}
		else {
			stackLDFS.pop();
		}
	}
	return ans;
}

void refresh();

bool IDS() {
	int limit = 0;
	bool ans = false;
	while (ans != true) {
		refresh();
		while (!stackPath.empty()) {
			stackPath.pop();
		}
		ans = LimitedDFS(limit);
		limit++;
	}
	return ans;
}

void refresh() {
	for (int i = 0;i < row;i++)
		for (int j = 0;j < col;j++) {
			if (grid[i][j] == 4)
				grid[i][j] = 0;
			visited[i][j] = false;
		}
	currentstate = initialstate;
	currentstate.cost = 0;

	grid[initialstate.x][initialstate.y] = 2;//representing 2 as initial state
	grid[goalstate.x][goalstate.y] = 3;//representing 3 as goal state
}

int main() {
	State tempB, tempA;
	inputFile("grid.txt");
	printGrid();

	if (BFS()) {
		cout << "\nPath found through BFS with a cost of: ";
		tempA = stackPath.top();
		stackPath.pop();
		cout << tempA.cost << endl;
		grid[tempA.x][tempA.y] = 4;
		while (!stackPath.empty()) {
			tempB=stackPath.top();
			if (tempA.parentx == tempB.x && tempA.parenty == tempB.y) {
				grid[tempB.x][tempB.y] = 4;
				tempA = tempB;
			}
			stackPath.pop();
		}
		grid[initialstate.x][initialstate.y] = 2;//representing 2 as initial state
		grid[goalstate.x][goalstate.y] = 3;//representing 3 as goal state
		printGrid();
	}
	else
		cout << "\n Path not found through BFS\n";

	refresh();

	if (DFS()) {
		cout << "\nPath found through DFS with a cost of: ";
		tempA = stackPath.top();
		stackPath.pop();
		cout << tempA.cost << endl;
		grid[tempA.x][tempA.y] = 4;
		while (!stackPath.empty()) {
			tempB = stackPath.top();
			if (tempA.parentx == tempB.x && tempA.parenty == tempB.y) {
				grid[tempB.x][tempB.y] = 4;
				tempA = tempB;
			}
			stackPath.pop();
		}
		grid[initialstate.x][initialstate.y] = 2;//representing 2 as initial state
		grid[goalstate.x][goalstate.y] = 3;//representing 3 as goal state
		printGrid();
	}
	else
		cout << "\n Path not found through DFS\n";

	refresh();

	if (IDS()) {
		cout << "\nPath found through IDS with a cost of: ";
		tempA = stackPath.top();
		stackPath.pop();
		cout << tempA.cost << endl;
		grid[tempA.x][tempA.y] = 4;
		while (!stackPath.empty()) {
			tempB = stackPath.top();
			if (tempA.parentx == tempB.x && tempA.parenty == tempB.y) {
				grid[tempB.x][tempB.y] = 4;
				tempA = tempB;
			}
			stackPath.pop();
		}
		grid[initialstate.x][initialstate.y] = 2;//representing 2 as initial state
		grid[goalstate.x][goalstate.y] = 3;//representing 3 as goal state
		printGrid();
	}
	else
		cout << "\n Path not found through IDS\n";

	refresh();
}