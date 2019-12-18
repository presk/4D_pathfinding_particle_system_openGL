#pragma once
#include "Board.h"


class Graph
{
public:
	Graph(Board * board, PivotCamera * camera);
	Graph();
	~Graph();
	void Render();
	void Update(float fDeltaTime);
	void findShortestPath();
	void findShortestBackwardPath();
	std::vector<Node*> getPath();
	void setPath(std::vector<Node *> path);



private:
	void GenerateGraph();
	Board * _board;
	std::vector<Node *> _path;
	PivotCamera * _camera;

};