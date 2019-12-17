#pragma once


class Node
{
public:
	Node(glm::vec3 pos, int type, bool _obstructed);
	~Node();
	void setType(int type);
	void setObstructed(bool obs);
	void addCNode(Node * n);
	void setPlayer(bool p);
	void setTarget(bool t);
	std::set<Node *> getCNodes();
	int getType();
	glm::vec3 getPosition();
	bool isObstructed();
	bool isPlayer();
	bool isTarget();


private:
	std::set<Node *> _connectedNodes;
	glm::vec3 _position;
	int _type;
	bool _isObstructed;
	bool _isTarget;
	bool _isPlayer;
	int _value;

};