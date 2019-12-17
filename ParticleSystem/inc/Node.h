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
	void setG(int g);
	void setH(int h);
	void setF(int f);
	void setParentNode(Node * n);
	std::set<Node *> getCNodes();
	int getType();
	int getG();
	int getH();
	int getF();
	Node * getParentNode();
	glm::vec3 getPosition();
	bool isObstructed();
	bool isPlayer();
	bool isTarget();
	bool operator<(const Node &n) const { return _f < n._f; }


private:
	std::set<Node *> _connectedNodes;
	Node * _parentNode;
	glm::vec3 _position;
	int _type;
	bool _isObstructed;
	bool _isTarget;
	bool _isPlayer;
	int _g;
	int _h;
	int _f;

};