/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	Alexia Soucy, 40014822
*
* SubTriangle is a class that defines the triangle halfs for a SubSquare(Quadrant)
*
* Team:
*	COMP477 F19, Team 8
*/

#pragma once
#include "ParticleEffect.h"
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
	//Setters
	void setType(int type);
	int getType();
	void setObstructed(bool obs);
	//Getters
	std::vector<glm::vec3> getVertices();
	Node * getNode();
	//Flags
	bool isObstructed();
	std::vector<glm::vec3> _vertices;

	
private:
	
	Node *_node;
	bool _obstructed;
	int _type;
};