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
	bool Update(float fDeltaTime, std::vector<Node *> path);
	std::vector<glm::vec3> getAllRoadTriangles();
	std::vector<Node*> getAllNodes();
	int getBoardSize();
	int getTileCount();
	glm::vec3 getAIPos();
	CrownGenerator * getAI();
	Node * getAINode();
	Node * getCheckpoint();
	void setCheckpoint(Node * n);
	void setAINode(Node * n);

private:
	
	std::vector<Tile *> _tiles;
	int _boardSize;
	int _tileCount;
	CrownGenerator * _AI;
	Node * _checkpoint;
	Node *_AINode;
	PivotCamera * _camera;

};