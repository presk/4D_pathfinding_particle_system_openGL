#pragma once
//#include "Tile.h"
#include "SubTriangle.h"

class SubSquare
{
public:
	SubSquare(glm::vec3 pos, float size/*, Tile * tile*/);
	~SubSquare();
	SubTriangle * getUpTriangle();
	SubTriangle * getDownTriangle();
	std::vector<SubTriangle*> _halfs; //0 = top, 1 = bottom

private:
	glm::vec3 _position;
	
	float _size;
	//Tile * _tile;

};