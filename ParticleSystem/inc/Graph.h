#pragma once
#include "Board.h"




class Graph
{
public:
	Graph(Board * board, PivotCamera * camera/*, ObstacleSet * obstacles*/);
	Graph();
	~Graph();
	void Render();
	void Update();
	void findShortestPath();
	std::vector<Node*> getPath();
	void setPath(std::vector<Node *> path);
	//void setNodeObstructed();
	bool _PathIsSet;
	bool _targetReached;

private:
	void GenerateGraph();
	Board * _board;
	std::vector<Node *> _path;
	PivotCamera * _camera;
	
	//ObstacleSet * _obstacles;

};