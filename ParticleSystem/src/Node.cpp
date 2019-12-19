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
#include "Node.h"

Node::Node(glm::vec3 pos, int type, bool obs)
{
	_position = pos;
	_type = type;
	_isObstructed = obs;
	_isPlayer = false;
	_isTarget = false;
	_parentNode = NULL;
	_wasUsed = false;
}

Node::~Node()
{
}

void Node::setType(int type)
{
	_type = type;
}

void Node::setObstructed(bool obs)
{
	_isObstructed = obs;
}

void Node::addCNode(Node * n)
{
	_connectedNodes.push_back(n);
}

void Node::setPlayer(bool p)
{
	_isPlayer = p;
}

void Node::setTarget(bool t)
{
	_isTarget = t;
}

void Node::setUsed(bool u)
{
	_wasUsed = u;
}

void Node::setG(int g)
{
	_g = g;
}

void Node::setH(int h)
{
	_h = h;
}

void Node::setF(int f)
{
	_f = f;
}

void Node::setParentNode(Node * n)
{
	_parentNode = n;
}

std::vector<Node*> Node::getCNodes()
{
	return _connectedNodes;
}

int Node::getType()
{
	return _type;
}

int Node::getG()
{
	return _g;
}

int Node::getH()
{
	return _h;
}

int Node::getF()
{
	return _f;
}

Node * Node::getParentNode()
{
	return _parentNode;
}


glm::vec3 Node::getPosition()
{
	return _position;
}

bool Node::isObstructed()
{
	return _isObstructed;
}

bool Node::isPlayer()
{
	return _isPlayer;
}

bool Node::isTarget()
{
	return _isTarget;
}

bool Node::wasUsed()
{
	return _wasUsed;
}
