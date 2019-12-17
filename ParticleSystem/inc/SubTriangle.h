#pragma once
#include "ParticleEffect.h"
#include "LampGenerator.h"
#include "PivotCamera.h"
#include "Node.h"

const int ROAD = 0;
const int ELSE = 1;

class SubTriangle
{
public:
	//SubTriangle(std::vector<glm::vec3> vertices, int type, bool obs);
	SubTriangle(std::vector<glm::vec3> vertices);
	~SubTriangle();

	void setType(int type);
	int getType();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec3> _vertices;
	void setObstructed(bool obs);
	bool isObstructed();
	Node * getNode();

private:
	Node *_node;
	bool _obstructed;
	int _type;
};