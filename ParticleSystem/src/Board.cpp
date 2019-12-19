#include "ParticleSystemPCH.h"
#include "Board.h"
#include "..\inc\Board.h"
//#include "CrownGenerator.h"

Board::Board(int bSize, int tCount)
{
	_boardSize = bSize;
	_tileCount = tCount;
	//std::cout << "Board Creation Success!" << std::endl;
	float tSize = (float)bSize / (float)tCount;
	for (int i = 0; i < tCount; i++)
	{
		for (int j = 0; j < tCount; j++)
		{
			Tile * t = new Tile(glm::vec3(tSize/2.0f - bSize/2.0f + j*tSize, 0, tSize / 2.0f - bSize / 2.0f + i*tSize), tSize);
			_tiles.push_back(t);
		}
	}
}

Board::Board()
{
}

Board::~Board()
{
	for (int i = 0; i < _tiles.size(); i++)
	{
		delete _tiles[i];
	}
}

void Board::Render()
{
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glm::vec3 translate = _camera->GetPivot();
	glTranslatef(translate.x, translate.y, translate.z);
	//glScalef(fScale, fScale, fScale);
	glColor3f(0.0f, 1.0f, 0.0f);
	std::vector<glm::vec3> roadTriangles = getAllRoadTriangles();

	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < roadTriangles.size(); i++)
		{
			if (i % 6 < 3)
			{
				glColor3f(0.0f, 1.0f, 0.0f);
			}
			else
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			glVertex3f(roadTriangles[i].x, roadTriangles[i].y, roadTriangles[i].z);
		}
	}
	glEnd();
	glPointSize(40.0f);//set point size to 10 pixels
	glColor3f(0.0f, 0.0f, 1.0f); //blue color
	glBegin(GL_POINTS);
	{
		glm::vec3 cPos = _checkpoint->getPosition();
		glVertex3f(cPos.x, 2.0f, cPos.z);
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
}

void Board::AttachCamera(PivotCamera * cam)
{
	_camera = cam;
}

void Board::AttachAI(CrownGenerator * cg)
{
	_AI = cg;
	std::vector<Node *> nodes = getAllNodes();
	int randInt = rand() % (7)  + 3;
	_AINode = nodes[_tileCount * 7];
	_AINode->setPlayer(true);
	glm::vec3 pos = _AINode->getPosition();
	_AI->Origin = pos + glm::vec3(0, 10.0f, 0);
	Node * n = nodes[nodes.size()-1 - 4 * _tileCount - randInt];
	setCheckpoint(n);

}

void Board::RandomizeTileType()
{
	
	std::vector<glm::vec3> roadTriangles;
	for (int i = 0; i < _tiles.size(); i++)
	{
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		for (int j = 0; j < ss.size(); j++)
		{
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			stu->setType(ROAD);
			stu->getNode()->setType(ROAD);
			stdown->setType(ROAD);
			stdown->getNode()->setType(ROAD);
			if (rand() % 10  < 3 && !(stu->getNode()->isObstructed()) && !(stu->getNode()->isPlayer()) && !(stu->getNode()->isTarget()))
			{
				stu->setType(ELSE);
				stu->getNode()->setType(ELSE);
			}
			if (rand() % 10  < 3 && !(stdown->getNode()->isObstructed()) && !(stdown->getNode()->isPlayer()) && !(stdown->getNode()->isTarget()))
			{
				stdown->setType(ELSE);
				stdown->getNode()->setType(ELSE);
			}
		}
	}

}

bool Board::Update(std::vector<Node *> path)
{
	if (path.size() > 1) 
	{
		glm::vec3 dist = glm::vec3(path[1]->getPosition().x - _AI->Origin.x, 0.0f, path[1]->getPosition().z - _AI->Origin.z);
		if (glm::length(dist) < 1)
		{
			setAINode(path[1]);
			return true;
		}

		else
		{
			dist.x = dist.x / 20;
			dist.z = dist.z / 20;
			_AI->Origin.x += dist.x;
			_AI->Origin.z += dist.z;
			//g_CrownGenerator.Origin.x += 0.1*distance;
			return false;
		}
	}
	
	
}

std::vector <glm::vec3> Board::getAllRoadTriangles()
{
	std::vector<glm::vec3> roadTriangles;
	for (int i = 0; i < _tiles.size(); i++)
	{
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		for (int j = 0; j < ss.size(); j++)
		{
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			if (stu->getType() == ROAD)
			{
				roadTriangles.push_back(stu->getVertices()[0]);
				roadTriangles.push_back(stu->getVertices()[1]);
				roadTriangles.push_back(stu->getVertices()[2]);
			}
			if (stdown->getType() == ROAD)
			{
				roadTriangles.push_back(stdown->getVertices()[0]);
				roadTriangles.push_back(stdown->getVertices()[1]);
				roadTriangles.push_back(stdown->getVertices()[2]);
			}
		}
	}
	return roadTriangles;
}

std::vector<Node*> Board::getAllNodes()
{
	std::vector<Node*> Triangles;
	for (int i = 0; i < _tiles.size(); i++)
	{
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		for (int j = 0; j < ss.size(); j++)
		{
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			roadTriangles.push_back(stu->getNode());
			roadTriangles.push_back(stdown->getNode());
		}
	}
	return Triangles;
}

int Board::getBoardSize()
{
	return _boardSize;
}

int Board::getTileCount()
{
	return _tileCount;
}

glm::vec3 Board::getAIPos()
{
	return _AI->Origin;
}

CrownGenerator * Board::getAI()
{
	return _AI;
}

Node * Board::getAINode()
{
	return _AINode;
}

Node * Board::getCheckpoint()
{
	return _checkpoint;
}

void Board::setCheckpoint(Node * n)
{
	n->setTarget(true);
	_checkpoint = n;
}

void Board::setAINode(Node * n)
{
	_AINode = n;
}
