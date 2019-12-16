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
}

SubTriangle::~SubTriangle()
{
}

void SubTriangle::setType(int type)
{
	_type = type;
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
}

bool SubTriangle::isObstructed()
{
	return _obstructed;
}




