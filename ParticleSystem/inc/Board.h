/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	Alexia Soucy, 40014822
*	
* Team:
*	COMP477 F19, Team 8
*
* Board is a class that holds the first layer of the game. It is subdivised following
* this pattern: Board->Tiles->SubSquares(Quadrant)->SubTriangles(Halfs).
* It also serves to initializes the demo by randomly selecting tile types.
*/

#pragma once
#include "Tile.h"
#include "CrownGenerator.h"


class Board
{
public:
	Board(int bSize, int tCount);					//Initializes the board and tiles
	Board();										//Default constructor
	~Board();										//Destructor
	void Render();									//Render method for openGL (Draws the board)
	void AttachCamera(PivotCamera * cam);			//Sets pointer to currently used camera (used for Render())
	void AttachAI(CrownGenerator * cg);				//Sets AI, gives it its initial position, sets random checkpoint(target)
	//Setters
	void setCheckpoint(Node * n);					
	void setAINode(Node * n);						
	
	void RandomizeTileType();						//Randomizes the tiles to get a board with unpathable tiles
	bool Update(std::vector<Node *> path);			//Checks if AI is at next node if it is update course, otherwise update its position
	
	//Getters
	std::vector<glm::vec3> getAllRoadTriangles();	
	std::vector<glm::vec3> getAllElseTriangles();	
	std::vector<Node*> getAllNodes();				
	int getBoardSize();								//Returns the size of the board(MxM board, returns M)
	int getTileCount();								//Returns the amount of tiles for a side(NxN tiles board, returns N)
	glm::vec3 getAIPos();							
	CrownGenerator * getAI();						
	Node * getAINode();								//Returns the last node the AI pathed on
	Node * getCheckpoint();							


private:
	
	std::vector<Tile *> _tiles;
	int _boardSize;
	int _tileCount;
	CrownGenerator * _AI;
	Node * _checkpoint;
	Node *_AINode;
	PivotCamera * _camera;

};