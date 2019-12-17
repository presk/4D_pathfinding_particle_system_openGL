#include "ParticleSystemPCH.h"
#include "Node.h"

Node::Node(glm::vec3 pos, int type, bool obs)
{
	_position = pos;
	_type = type;
	_isObstructed = obs;
}

Node::~Node()
{
	/*for (auto n : _connectedNodes)
	{
		delete n;
	}*/
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
	_connectedNodes.insert(n);
}

void Node::setPlayer(bool p)
{
	_isPlayer = p;
}

void Node::setTarget(bool t)
{
	_isTarget = t;
}

std::set<Node*> Node::getCNodes()
{
	return _connectedNodes;
}

int Node::getType()
{
	return _type;
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
