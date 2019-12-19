/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*
*
*
* Team:
*	COMP477 F19, Team 8
*
* Resource used:
* http://theory.stanford.edu/~amitp/GameProgramming/ImplementationNotes.html
*/

#include "ParticleSystemPCH.h"
#include "Graph.h"

Graph::Graph(Board * board, PivotCamera * camera/*, ObstacleSet * o*/)
{
	_board = board;
	_camera = camera;
	_PathIsSet = false;
	_targetReached = false;
	GenerateGraph();
}

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::Render()
{
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glm::vec3 translate = _camera->GetPivot();
	glTranslatef(translate.x, translate.y, translate.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	{
		//Draws lines between the nodes of the A* path
		glColor3f(1.0f, 1.0f, 1.0f);
		if (!_path.empty())
		{
			for (int i = 0; i < _path.size() - 1; i++)
			{
				glm::vec3 pos = _path[i]->getPosition();
				glVertex3f(pos.x, pos.y + 1.0f, pos.z);
				glm::vec3 pos2 = _path[i + 1]->getPosition();
				glVertex3f(pos2.x, pos2.y + 1.0f, pos2.z);
			}
		}
		
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
}

void Graph::Update()
{
	//Only update if a A* path has already been calculated and if the target hasn't been reached
	if (!_targetReached && _PathIsSet && _path.size() > 1)
	{
		//Checks if the AI has reached the target
		if (_path[1]->isTarget())
		{
			_targetReached = true;
			std::cout << "Target Reached !" << std::endl;
		}
		//Else if the AI has reached the next node, set AI(player) at the next node and recalc A*
		else if (_board->Update(_path))
		{
			_path[0]->setPlayer(false);
			_path[0]->setUsed(true);
			_path[1]->setPlayer(true);
			findShortestPath();
		}
	}
	//If there are no path and target has not been reached calculate path
	else if (!_targetReached)
	{
		findShortestPath();
	}
	
}

void Graph::findShortestPath()
{
	std::vector<Node *> nodes = _board->getAllNodes();
	Node * startNode = _board->getAINode();
	Node * targetNode = _board->getCheckpoint();
	//Initialize all nodes with the proper F(), G() and H()
	for (auto n : nodes)
	{
		if (!(n->isPlayer()))
		{
			glm::vec3 dist = targetNode->getPosition() - n->getPosition();
			glm::vec3 dist2 = startNode->getPosition() - n->getPosition();
			glm::vec3 dist3 = targetNode->getPosition() - startNode->getPosition();
			int backVal = 0;
			if (glm::length(dist3) < glm::length(dist))
			{
				backVal = 100;
			}
			//Set Heuristic as distance to AI + distance to target + value if behind AI to limit wrap arounds
			n->setH((int)glm::length(dist) + (int)glm::length(dist) + backVal);
			//Set F() as "infinity"
			n->setF(9999999999999);
			//Set G() as 0
			n->setG(0);
		}
		n->setParentNode(NULL);
	}
	//Create open and closed list (Note: multiset compare operator < is defined in Node)
	std::multiset<Node *> openList;
	std::multiset<Node *> closedList;
	//Initializes start node and adds it to the open list
	startNode->setF(0);
	startNode->setG(0);
	openList.insert(startNode);
	while (!openList.empty())
	{
		//Select then remove first item from open add it to close
		std::multiset<Node *>::iterator it;
		it = openList.begin();
		Node * node = *it;
		openList.erase(node);
		closedList.insert(node);
		//Get all the connected Nodes
		std::vector<Node *> nodeSet = node->getCNodes();
		for (auto nextNode : nodeSet)
		{
			if (nextNode->isTarget())
			{
				nextNode->setParentNode(node);
				break;
			}
			if (/*!nextNode->wasUsed()*/ true) //Condition used for debugging
			{
				int oldG = nextNode->getG();
				//Adds value to G() if the node is obstructed to make the AI avoid obstructed nodes
				int newG = node->getG() + 1 + 300 * nextNode->isObstructed();
				//If node is in Open and new G() is smaller, remove the node with old G() from open
				if (openList.find(nextNode) != openList.end() && newG < oldG)
				{
					openList.erase(nextNode);
				}
				//If node is in Closed and new G() is smaller, remove the node with old G() from open
				if (closedList.find(nextNode) != closedList.end() && newG < oldG)
				{
					closedList.erase(nextNode);
				}
				//If node is not in open or closed set its G() to the calculated G(), 
				//set parent node as previous node and add to Open
				if (openList.find(nextNode) == openList.end() && closedList.find(nextNode) == closedList.end())
				{
					nextNode->setG(newG);
					nextNode->setF(nextNode->getG() + nextNode->getH());
					nextNode->setParentNode(node);
					openList.insert(nextNode);
				}
			}
			
		}
	}
	Node * p = targetNode;
	std::vector<Node *> path;
	
	//Checks if a path has been found
	if (targetNode->getParentNode() != NULL)
	{
		//Retrace path using parent Node from Target to Player(AI)
		path.push_back(p);
		bool playerFound = false;
		while (!playerFound)
		{
			Node * temp = p->getParentNode();
			p = temp;
			path.push_back(p);
			if (p->isPlayer()) {playerFound = true;}
		}
		std::vector<Node *> path2;
		//Path is from end to beginning, path2 iterates it backwards to have beginning to end
		for (int i = path.size() - 1; i >= 0; i--)
		{
			path2.push_back(path[i]);
		}
		std::cout << "Path size: " << path.size() << std::endl;
		_path = path2;
		_PathIsSet = true;
	}
	//If path hasnt been set once already and there is no solution, shuffle the board
	else if(!_PathIsSet)
	{
		std::cout << "no solution" << std::endl;
		_board->RandomizeTileType();
		GenerateGraph();
	}
}


std::vector<Node*> Graph::getPath()
{
	return _path;
}

void Graph::setPath(std::vector<Node *> path)
{
	_path = path;
}


void Graph::GenerateGraph()
{
	//Tile Oder
	/*
	*2		5		8
	*
	*1		4		7
	*
	*0		3		6
	*/
	//SubSquare Order
	/*
	*|-----------|----------|
	*|			2|		   6|
	*|	   (1)	 |	   (3)	|
	*| 3		 |  7		|
	*|-----------|----------|
	*|			0|		   4|
	*|	   (0)	 |	  (2)	|
	*|  1		 |  5		|
	*|-----------|----------|
	*/
	//Triangle Order
	/*---------------
	*\			   |
	*	\		0  |
	*		\	   |
	*	1		\  |
	*//*----------\|
	*/
	/*
	*
	*
	*
	*/
	int tCount = _board->getTileCount();
	std::vector<Node *> nodes = _board->getAllNodes();
	//Connects all the adjacent nodes
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getType() == ROAD)
		{
			//Finds the connected ROAD nodes depending on which triangle the node is on
			switch (i % 8)
			{
				//See subSquare Order numbers without bracket for the triangle numbers in this switch-case
				case 0:
					if (nodes[i + 3]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 3]);
					}
					if (nodes[i + 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1]);
					}
					if (nodes[i + 5]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 5]);
					}
					break;
				case 1:
					//Tile is below so previous tile =  - 8 triangles
					if ((i + 1 - 8) > 0 && i % (tCount*8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1 - 8]);
					}
					//Tile is left of current tile (so 1 row previous = - 8* tCount)
					if ((i + 3 - (8 * tCount)) > 0 && nodes[i + 3 - 8 * tCount]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 3 - 8 * tCount]);
					}
					if (nodes[i - 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1]);
					}
					break;
				case 2:
					if (nodes[i + 5]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 5]);
					}
					if (nodes[i + 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1]);
					}
					//Tile is over so next tile = + 8 triangles
					if ((i - 1 + 8) < nodes.size() && i % (tCount * 8) < 32 && nodes[i - 1 + 8]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1 + 8]);
					}
					break;
				case 3:
					if (nodes[i - 3]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 3]);
					}
					if (nodes[i - 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1]);
					}
					//Tile is left of current tile (so 1 row previous = - 8* tCount)
					if ((i + 3 - (8 * tCount)) > 0  && nodes[i + 3 - (8 * tCount)]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 3 - (8 * tCount)]);
					}
					break;

				case 4:
					if (nodes[i + 3]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 3]);
					}
					if (nodes[i + 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1]);
					}
					//Tile is right of current tile (so 1 row after = + 8* tCount)
					if ((i - 3 + 8 * tCount) < nodes.size() && nodes[i - 3 + 8 * tCount]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 3 + 8 * tCount]);
					}
					break;

				case 5:
					if (nodes[i - 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1]);
					}
					if (nodes[i - 5]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 5]);
					}
					//Tile is below so previous tile =  - 8 triangles
					if ((i + 1 - 8) > 0 && i % (tCount * 8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1 - 8]);
					}
					break;

				case 6:
					//Tile is over so next tile = + 8 triangles
					if ((i - 1 + 8) < nodes.size() && i % (tCount * 8) < 32 && nodes[i - 1 + 8]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1 + 8]);
					}
					if (nodes[i + 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i + 1]);
					}
					//Tile is right of current tile (so 1 row after = + 8* tCount)
					if ((i - 3 + 8 * tCount) < nodes.size() && nodes[i - 3 + 8 * tCount]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 3 + 8 * tCount]);
					}
					break;

				case 7:
					if (nodes[i - 3]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 3]);
					}
					if (nodes[i - 1]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 1]);
					}
					if (nodes[i - 5]->getType() == ROAD)
					{
						nodes[i]->addCNode(nodes[i - 5]);
					}
					break;
			}
		}	
	}
	//Once all the nodes are connected, find the shortest path
	findShortestPath();
}


