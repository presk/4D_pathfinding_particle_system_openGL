/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
*
*
* Team:
*	COMP477 F19, Team 8
*
* Graph is a class that connects all the nodes together then generates and holds the 
* shortest path found using A*.
* Resource used:
* http://theory.stanford.edu/~amitp/GameProgramming/ImplementationNotes.html
*/

#pragma once
#include "Board.h"




class Graph
{
public:
	Graph(Board * board, PivotCamera * camera); //Initializes the graph
	Graph();									//Default Constructor
	~Graph();									//Destructor
	void Render();								//Render method for openGL (draws the A* path)
	void Update();								//Checks AI states and recalculates if a change has happened
	void findShortestPath();					//Implements A* with simple Heuristic
	std::vector<Node*> getPath();				//Returns the last calculated A* path
	void setPath(std::vector<Node *> path);		//Sets Path
	bool _PathIsSet;							//Bool if Path has been calculated once
	bool _targetReached;						//Bool if target is reached

private:
	void GenerateGraph();						//Generates the initial graph by connecting nodes
	Board * _board;
	std::vector<Node *> _path;
	PivotCamera * _camera;
};