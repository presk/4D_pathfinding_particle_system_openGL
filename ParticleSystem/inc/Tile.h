#pragma once
#include "SubSquare.h"

class Tile
{
public:
	Tile(glm::vec3 pos, float size);
	~Tile();

	std::vector<SubSquare *> getAllSquares();

private:
	glm::vec3 _position;
	std::vector<SubSquare *> _quadrants;
	float _size;

};