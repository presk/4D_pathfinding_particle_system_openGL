#include "ParticleSystemPCH.h"
#include "SubSquare.h"
#include "..\inc\SubSquare.h"

SubSquare::SubSquare(glm::vec3 pos,float size/*, Tile * tile*/)
{
	//std::cout << "Square Creation Success!" << std::endl;
	_position = pos;
	_size = size;
	/*_tile = tile;*/
	float offset = _size / 2;
	std::vector<glm::vec3> sqVertices;
	std::vector<glm::vec3> upVertices;
	std::vector<glm::vec3> downVertices;
	/*
	*1			3
	*
	*0			2				
	*/
	for (int i = -1; i <= 1; i = i + 2)
	{
		for (int j = -1; j <= 1; j = j + 2)
		{
			glm::vec3 sqVertex = _position + glm::vec3(i * offset, 0, j * offset);
			sqVertices.push_back(sqVertex);
		}
	}
	upVertices.push_back(sqVertices[2]);
	upVertices.push_back(sqVertices[3]);
	upVertices.push_back(sqVertices[1]);
	downVertices.push_back(sqVertices[1]);
	downVertices.push_back(sqVertices[0]);
	downVertices.push_back(sqVertices[2]);

	SubTriangle * stUP = new SubTriangle(upVertices);
	SubTriangle * stDOWN = new SubTriangle(downVertices);
	_halfs.push_back(stUP);
	_halfs.push_back(stDOWN);
}

SubSquare::~SubSquare()
{
	for (int i = 0; i < _halfs.size(); i++)
	{
		delete _halfs[i];
	}
}


SubTriangle * SubSquare::getUpTriangle()
{
	return  _halfs[0];
}

SubTriangle * SubSquare::getDownTriangle()
{
	return _halfs[1];
}
