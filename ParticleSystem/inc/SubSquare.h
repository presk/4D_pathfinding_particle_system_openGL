/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
* SubSquare is a class that defines the Quadrants of a tile.
* It holds its two triangle halfs
*
* Team:
*	COMP477 F19, Team 8
*/

#pragma once
//#include "Tile.h"
#include "SubTriangle.h"

class SubSquare
{
public:
	SubSquare(glm::vec3 pos, float size);
	~SubSquare();
	//Getters
	SubTriangle * getUpTriangle();
	SubTriangle * getDownTriangle();

	

private:
	glm::vec3 _position;
	std::vector<SubTriangle*> _halfs; //0 = top, 1 = bottom
	float _size;
	//Tile * _tile;

};