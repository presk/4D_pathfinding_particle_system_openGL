/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
* Tile is a class that defines the tiles in a board.
* It holds its 4 subSquare Quadrants
*
* Team:
*	COMP477 F19, Team 8
*/

#pragma once
#include "SubSquare.h"

class Tile
{
public:
	Tile(glm::vec3 pos, float size);
	~Tile();
	//Getter
	std::vector<SubSquare *> getAllSquares();

private:
	glm::vec3 _position;
	std::vector<SubSquare *> _quadrants;
	float _size;

};