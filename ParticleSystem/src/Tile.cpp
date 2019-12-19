/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
*
*
* Team:
*	COMP477 F19, Team 8
*/

#include "ParticleSystemPCH.h"
#include "Tile.h"
#include "..\inc\Tile.h"

Tile::Tile(glm::vec3 pos, float size)
{
	_position = pos;
	_size = size;
	float offset = _size / 4;
	/*
	*|-----------|----------|
	*|		1	 |		3	|
	*|-----------|----------|
	*|		0	 |		2	|
	*|-----------|----------|	
	*/
	//Generates the subSquare(Quadrants) for the tile
	for (int i = -1; i <= 1; i = i + 2) 
	{
		for(int j = -1; j <= 1; j = j + 2)
		{
			glm::vec3 ssPos = _position + glm::vec3(j * offset, 0, i * offset);
			SubSquare * ss = new SubSquare(ssPos, offset * 2);
			_quadrants.push_back(ss);
		}
	}
}

Tile::~Tile()
{
	for (int i = 0; i < _quadrants.size(); i++)
	{
		delete _quadrants[i];
	}
}


std::vector<SubSquare*> Tile::getAllSquares()
{
	return  _quadrants;
}
