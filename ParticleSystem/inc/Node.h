/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
* Node is a class that creates and holds a node. The node has
* multiple flags: _type which defines if it is ROAD or ELSE,
* _isObstructed which defines if a 4D object is on it,
* _isTarget which defines is this node is the target node,
* _isPlayer which defines if this node is that last node the AI(player) was on,
* _wasUsed which is used for debugging but checks if the node has been used already.
* Each node also has a set of connected nodes, a parent node, F(), G(), H() 
* used to calculate the path using A*.
*
* Team:
*	COMP477 F19, Team 8
*/

#pragma once


class Node
{
public:
	Node(glm::vec3 pos, int type, bool _obstructed);
	~Node();
	//Setters
	void setType(int type);
	void setObstructed(bool obs);
	void addCNode(Node * n);
	void setPlayer(bool p);
	void setTarget(bool t);
	void setUsed(bool u);
	void setG(int g);
	void setH(int h);
	void setF(int f);
	void setParentNode(Node * n);
	//Getters
	std::vector<Node *> getCNodes();  //Returns the connected nodes
	int getType();
	int getG();
	int getH();
	int getF();
	Node * getParentNode();
	glm::vec3 getPosition();
	//Flags
	bool isObstructed();
	bool isPlayer();
	bool isTarget();
	bool wasUsed();
	//Compare operator for multisets
	bool operator<(const Node &n) const { return _f < n._f; }


private:
	std::vector<Node *> _connectedNodes;
	Node * _parentNode;
	glm::vec3 _position;
	int _type;
	bool _isObstructed;
	bool _isTarget;
	bool _isPlayer;
	bool _wasUsed;
	int _g;
	int _h;
	int _f;

};