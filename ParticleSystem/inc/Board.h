#pragma once
#include "Tile.h"


class Board
{
public:
	Board(int bSize, int tCount);
	Board();
	~Board();
	void Render();
	void AttachCamera(PivotCamera * cam);
	void RandomizeTileType();
	std::vector<glm::vec3> getAllRoadTriangles();

private:
	
	std::vector<Tile *> _tiles;
	int _boardSize;
	int _tileCount;
	PivotCamera * _camera;

};