/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	Alexia Soucy, 40014822
*
*
* Team:
*	COMP477 F19, Team 8
*/

#include "ParticleSystemPCH.h"
#include "SubTriangle.h"
#include "..\inc\SubTriangle.h"



SubTriangle::SubTriangle(std::vector<glm::vec3> vertices)
{
	_vertices = vertices;
	_obstructed = false;
	_type = ROAD;
	float sumx = 0;
	float sumz = 0;
	
	for (int i = 0; i < 3; i++)
	{
		sumx += _vertices[i].x;
		sumz += _vertices[i].z;
	}
	glm::vec3 pos = glm::vec3(sumx/3, 0.01, sumz/3);
	_node = new Node(pos, _type, _obstructed);
}

SubTriangle::~SubTriangle()
{
	delete _node;
}

void SubTriangle::setType(int type)
{
	_type = type;
	_node->setType(type);
}

int SubTriangle::getType()
{
	return _type;
}

std::vector<glm::vec3> SubTriangle::getVertices()
{
	return _vertices;
}

void SubTriangle::setObstructed(bool obs)
{
	_obstructed = obs;
	_node->setObstructed(obs);
}

bool SubTriangle::isObstructed()
{
	return _obstructed;
}

Node * SubTriangle::getNode()
{
	return _node;
}




