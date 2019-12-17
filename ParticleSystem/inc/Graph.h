#pragma once
#include "Board.h"


class Graph
{
public:
	Graph(Board * board, PivotCamera * camera);
	Graph();
	~Graph();
	void Render();



private:
	void GenerateGraph();
	Board * _board;

	PivotCamera * _camera;

};