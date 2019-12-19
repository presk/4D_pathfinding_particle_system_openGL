#include "ParticleSystemPCH.h"
#include "Graph.h"

Graph::Graph(Board * board, PivotCamera * camera/*, ObstacleSet * o*/)
{
	_board = board;
	_camera = camera;
	//_obstacles = o;
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
	//glScalef(fScale, fScale, fScale);
	glColor3f(1.0f, 1.0f, 1.0f);
	//std::vector<glm::vec3> roadTriangles = getAllRoadTriangles();
	//std::vector<Node*> nodes = _board->getAllNodes();
	glBegin(GL_LINES);
	{
		/*for (int i = 0; i < nodes.size(); i++)
		{
			std::vector<Node *> cNodes = nodes[i]->getCNodes();
			for (auto n : cNodes)
			{
				glm::vec3 pos = nodes[i]->getPosition();
				glVertex3f(pos.x, pos.y, pos.z);
				glm::vec3 pos2 = n->getPosition();
				glVertex3f(pos2.x, pos2.y, pos2.z);
			}
		}*/
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
	if (!_targetReached && _PathIsSet && _path.size() > 1)
	{
		if (_path[1]->isTarget())
		{
			_targetReached = true;
			std::cout << "Target Reached !" << std::endl;
		}
		else if (_board->Update(_path))
		{
			_path[0]->setPlayer(false);
			_path[0]->setUsed(true);
			_path[1]->setPlayer(true);
			findShortestPath();
			//_board->getAI()->SetCourse(_path[1]->getPosition());
		}
	}
	else if (!_targetReached)
	{
		findShortestPath();
	}
	
}

void Graph::findShortestPath()
{
	//std::cout << "Finding Path" << std::endl;
	std::vector<Node *> nodes = _board->getAllNodes();
	Node * startNode = _board->getAINode();
	Node * targetNode = _board->getCheckpoint();
	//_PathIsSet = false;
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
			n->setH((int)glm::length(dist) + (int)glm::length(dist) + backVal);
			//n->setH(1);
			n->setF(9999999999999);
			n->setG(0);;
		}
		n->setParentNode(NULL);
	}
	std::multiset<Node *> openList;
	std::multiset<Node *> closedList;
	startNode->setF(0);
	startNode->setG(0);
	openList.insert(startNode);
	while (!openList.empty())
	{
		std::multiset<Node *>::iterator it;
		it = openList.begin();
		Node * node = *it;
		openList.erase(node);
		closedList.insert(node);
		std::vector<Node *> nodeSet = node->getCNodes();
		for (auto nextNode : nodeSet)
		{
			if (nextNode->isTarget())
			{
				nextNode->setParentNode(node);
				break;
			}
			if (!nextNode->wasUsed()) 
			{
				int oldG = nextNode->getG();
				int newG = node->getG() + 1 + 300 * nextNode->isObstructed();
				if (openList.find(nextNode) != openList.end() && newG < oldG)
				{
					openList.erase(nextNode);
				}
				if (closedList.find(nextNode) != closedList.end() && newG < oldG)
				{
					closedList.erase(nextNode);
				}
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
	
	
	if (targetNode->getParentNode() != NULL)
	{
		path.push_back(p);
		while (p->getParentNode() != NULL)
		{

			Node * temp = p->getParentNode();
			p = temp;
			path.push_back(p);
		}
		std::vector<Node *> path2;
		for (int i = path.size() - 1; i >= 0; i--)
		{
			path2.push_back(path[i]);
		}
		std::cout << "Path size: " << path.size() << std::endl;
		_path = path2;
		_PathIsSet = true;
	}
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

/*void Graph::setNodeObstructed()
{
	
	for (auto n : _path)
	{
		if (_obstacles->testNode(n->getPosition()))
		{
			n->setObstructed(true);
		}
	}
}*/

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
	//int bSize = _board->getBoardSize();
	int tCount = _board->getTileCount();
	std::vector<Node *> nodes = _board->getAllNodes();
	//std::cout << "# of nodes: " << nodes.size() << std::endl;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getType() == ROAD)
		{
			switch (i % 8)
			{
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
				
				if ((i + 1 - 8) > 0 && i % (tCount*8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1 - 8]);
				}
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
				if ((i + 1 - 8) > 0 && i % (tCount * 8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1 - 8]);
				}
				break;
				case 6:
				
				if ((i - 1 + 8) < nodes.size() && i % (tCount * 8) < 32 && nodes[i - 1 + 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1 + 8]);
				}
				if (nodes[i + 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1]);
				}
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
	findShortestPath();
}


