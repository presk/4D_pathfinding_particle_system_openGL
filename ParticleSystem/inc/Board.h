#pragma once
#include "Tile.h"
#include "CrownGenerator.h"


class Board
{
public:
	Board(int bSize, int tCount);
	Board();
	~Board();
	void Render();
	void AttachCamera(PivotCamera * cam);
	void AttachAI(CrownGenerator * cg);
	void RandomizeTileType();
	std::vector<glm::vec3> getAllRoadTriangles();
	std::vector<Node*> getAllNodes();
	int getBoardSize();
	int getTileCount();
	glm::vec3 getAIPos();
	Node * getAINode();
	Node * getCheckpoint();
	void setCheckpoint(Node * n);

private:
	
	std::vector<Tile *> _tiles;
	int _boardSize;
	int _tileCount;
	CrownGenerator * _AI;
	Node * _checkpoint;
	Node *_AINode;
	PivotCamera * _camera;

};