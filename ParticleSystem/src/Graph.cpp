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
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
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
}


