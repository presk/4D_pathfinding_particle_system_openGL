#include "ParticleSystemPCH.h"
#include "SubTriangle.h"
#include "..\inc\SubTriangle.h"


/*SubTriangle::SubTriangle(std::vector<glm::vec3> vertices, int type, bool obs)
{
	_vertices = vertices;
	_type = type;
	_obstructed = obs;
}*/
SubTriangle::SubTriangle(std::vector<glm::vec3> vertices)
{
	//std::cout << "Triangle Creation Success!" << std::endl;
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




