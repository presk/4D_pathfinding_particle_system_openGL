#include "ParticleSystemPCH.h"
#include "Graph.h"

Graph::Graph(Board * board, PivotCamera * camera)
{
	_board = board;
	_camera = camera;
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
	std::vector<Node*> nodes = _board->getAllNodes();
	glBegin(GL_LINES);
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			std::set<Node *> cNodes = nodes[i]->getCNodes();
			for (auto n : cNodes)
			{
				glm::vec3 pos = nodes[i]->getPosition();
				glVertex3f(pos.x, pos.y, pos.z);
				glm::vec3 pos2 = n->getPosition();
				glVertex3f(pos2.x, pos2.y, pos2.z);
			}
		}
		glColor3f(1.0f, 0.0f, 0.0f);
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

void Graph::findShortestPath()
{
	std::vector<Node *> nodes = _board->getAllNodes();
	Node * startNode = _board->getAINode();
	Node * targetNode = _board->getCheckpoint();
	for (auto n : nodes)
	{
		if (!(n->isPlayer()))
		{
			glm::vec3 dist = targetNode->getPosition() - n->getPosition();
			n->setH((int)glm::length(dist));
			n->setF(99999999);
			n->setG(0);
			n->setParentNode(NULL);
		}
		
	}
	typedef std::pair<int, Node*> nodePair;
	//std::priority_queue<nodePair, std::vector<nodePair>, std::greater<nodePair> > openList;
	//std::priority_queue<nodePair, std::vector<nodePair>, std::greater<nodePair> > closedList;
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
		std::set<Node *> nodeSet = node->getCNodes();
		for (auto nextNode : nodeSet)
		{
			if (nextNode->isTarget())
			{
				nextNode->setParentNode(node);
				break;
			}
			int oldG = nextNode->getG();
			int newG = node->getG() + 1 + 1 * nextNode->isObstructed();
			if ( openList.find(nextNode) != openList.end() && newG < oldG)
			{
				openList.erase(nextNode);
			}
			if (closedList.find(nextNode) == closedList.end() && newG < oldG)
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
	Node * p = targetNode;
	std::vector<Node *> path;
	while(p->getParentNode() != NULL)
	{
		path.insert(path.begin(), p);
		p = p->getParentNode();
	}
	_path = path;
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
	//int bSize = _board->getBoardSize();
	int tCount = _board->getTileCount();
	std::vector<Node *> nodes = _board->getAllNodes();
	std::cout << "# of nodes: " << nodes.size() << std::endl;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getType() == ROAD)
		{
			switch (i % 8)
			{
				case 0:
				if (nodes[i + 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1]);
				}
				if (nodes[i + 3]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 3]);
				}
				if (nodes[i + 5]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 5]);
				}
				break;
				case 1:
				if ((i + 3 - (8 * tCount)) > 0 && nodes[i + 3 - 8 * tCount]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 3 - 8 * tCount]);
				}
				if ((i + 1 - 8) > 0 && i % (tCount*8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1 - 8]);
				}
				if (nodes[i - 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1]);
				}
				break;
				case 2:
				if (nodes[i + 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1]);
				}
				if (nodes[i + 5]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 5]);
				}
				if ((i - 1 + 8) < nodes.size() && i % (tCount * 8) < 32 && nodes[i - 1 + 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1 + 8]);
				}
				break;
				case 3:
				if (nodes[i - 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1]);
				}
				if (nodes[i - 3]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 3]);
				}
				if ((i + 3 - (8 * tCount)) > 0  && nodes[i + 3 - (8 * tCount)]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 3 - (8 * tCount)]);
				}
				break;
				case 4:
				if (nodes[i + 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1]);
				}
				if (nodes[i + 3]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 3]);
				}
				if ((i - 3 + 8 * tCount) < nodes.size() && nodes[i - 3 + 8 * tCount]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 3 + 8 * tCount]);
				}
				break;
				case 5:
				if (nodes[i - 5]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 5]);
				}
				if (nodes[i - 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1]);
				}
				if ((i + 1 - 8) > 0 && i % (tCount * 8) > 7 && nodes[i + 1 - 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1 - 8]);
				}
				break;
				case 6:
				if (nodes[i + 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i + 1]);
				}
				if ((i - 1 + 8) < nodes.size() && i % (tCount * 8) < 32 && nodes[i - 1 + 8]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1 + 8]);
				}
				if ((i - 3 + 8 * tCount) < nodes.size() && nodes[i - 3 + 8 * tCount]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 3 + 8 * tCount]);
				}
				break;
				case 7:
				if (nodes[i - 1]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 1]);
				}
				if (nodes[i - 3]->getType() == ROAD)
				{
					nodes[i]->addCNode(nodes[i - 3]);
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


