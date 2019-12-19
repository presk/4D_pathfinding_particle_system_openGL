/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	Alexia Soucy, 40014822
*
*
* Team:
*	COMP477 F19, Team 8
*
*/

#include "ParticleSystemPCH.h"
#include "Board.h"

Board::Board(int bSize, int tCount)
{
	_boardSize = bSize;
	_tileCount = tCount;
	//Gets the size for a tile
	float tSize = (float)bSize / (float)tCount;
	//Initializes all tiles
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
	glColor3f(0.0f, 1.0f, 0.0f);
	std::vector<glm::vec3> roadTriangles = getAllRoadTriangles();
	std::vector<glm::vec3> elseTriangles = getAllElseTriangles();

	glBegin(GL_TRIANGLES);
	{
		// Render road triangles
		for (int i = 0; i < roadTriangles.size(); i++)
		{
			glColor3f(0.86f, 0.78f, 0.64f);
			glVertex3f(roadTriangles[i].x, roadTriangles[i].y, roadTriangles[i].z);
		}

		// Render else triangles
		for (int i = 0; i < elseTriangles.size(); i+=3)
		{
			// Bottom triangle
			glm::vec3 bottomV1 = glm::vec3(elseTriangles[i].x, 0.0f, elseTriangles[i].z);
			glm::vec3 bottomV2 = glm::vec3(elseTriangles[i+1].x, 0.0f, elseTriangles[i+1].z);
			glm::vec3 bottomV3 = glm::vec3(elseTriangles[i+2].x, 0.0f, elseTriangles[i+2].z);
			// Top triangle
			glm::vec3 topV1 = glm::vec3(elseTriangles[i].x, elseTriangles[i].y, elseTriangles[i].z);
			glm::vec3 topV2 = glm::vec3(elseTriangles[i+1].x, elseTriangles[i+1].y, elseTriangles[i+1].z);
			glm::vec3 topV3 = glm::vec3(elseTriangles[i+2].x, elseTriangles[i+2].y, elseTriangles[i+2].z);

			// Get all boulder vertices
			std::vector<glm::vec3> boulder;
			boulder.push_back(bottomV1);
			boulder.push_back(bottomV2);
			boulder.push_back(topV2);
			boulder.push_back(bottomV1);
			boulder.push_back(topV2);
			boulder.push_back(topV1);
			boulder.push_back(bottomV2);
			boulder.push_back(bottomV3);
			boulder.push_back(topV3);
			boulder.push_back(bottomV2);
			boulder.push_back(topV3);
			boulder.push_back(topV2);
			boulder.push_back(bottomV3);
			boulder.push_back(bottomV1);
			boulder.push_back(topV1);
			boulder.push_back(bottomV3);
			boulder.push_back(topV1);
			boulder.push_back(topV3);
			boulder.push_back(topV1);
			boulder.push_back(topV2);
			boulder.push_back(topV3);

			// Draw all 7 triangles to create boulders with a lighter tint at the top
			for (int v = 0; v < boulder.size(); v++) {
				glColor3f(0.5f/10.0f * boulder[v].y, 0.49f / 10.0f * boulder[v].y, 0.46f / 10.0f * boulder[v].y);
				glVertex3f(boulder[v].x, boulder[v].y, boulder[v].z);
			}
		}
	}
	glEnd();

	glPointSize(40.0f);//set point size to 10 pixels
	glColor3f(0.0f, 0.0f, 1.0f); //blue color
	//Draws the checkpoint(target)
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
	//Randomizes offset for the checkpoint
	int randInt = rand() % (7)  + 3;
	//Places AI in a corner
	_AINode = nodes[_tileCount * 7];
	_AINode->setPlayer(true);
	glm::vec3 pos = _AINode->getPosition();
	_AI->Origin = pos + glm::vec3(0, 10.0f, 0);
	//Selects checkpoint node
	Node * n = nodes[nodes.size()-1 - 4 * _tileCount - randInt];
	setCheckpoint(n);

}

void Board::RandomizeTileType()
{
	

	//Gets all tiles
	for (int i = 0; i < _tiles.size(); i++)
	{
		//Gets all SubSquares(Quadrants)
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		//Gets all Triangle(Halfs)
		for (int j = 0; j < ss.size(); j++)
		{
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			//Resets all triangles and associated nodes to type ROAD
			stu->setType(ROAD);
			stu->getNode()->setType(ROAD);
			stdown->setType(ROAD);
			stdown->getNode()->setType(ROAD);

			// Change ROAD triangle height to 0
			for (int i = 0; i < stdown->_vertices.size(); i++) {
				stdown->_vertices[i] = glm::vec3(stdown->_vertices[i].x, 0.0f, stdown->_vertices[i].z);
			}
			for (int i = 0; i < stu->_vertices.size(); i++) {
				stu->_vertices[i] = glm::vec3(stu->_vertices[i].x, 0.0f, stu->_vertices[i].z);
			}

			//Randomly selects some triangles to become ELSE (non-pathable)
			if (rand() % 10  < 1 && !(stu->getNode()->isObstructed()) && !(stu->getNode()->isPlayer()) && !(stu->getNode()->isTarget()))
			{
				stu->setType(ELSE);
				stu->getNode()->setType(ELSE);

				// Change ELSE triangle height to a random value between 3 and 10
				float height = rand() % 70 / 10.0f + 3;
				for (int i = 0; i < stu->_vertices.size(); i++) {
					stu->_vertices[i] = glm::vec3(stu->_vertices[i].x, stu->_vertices[i].y + height, stu->_vertices[i].z);
				}
			}
			//Randomly selects some triangles to become ELSE (non-pathable)
			if (rand() % 10  < 1 && !(stdown->getNode()->isObstructed()) && !(stdown->getNode()->isPlayer()) && !(stdown->getNode()->isTarget()))
			{
				stdown->setType(ELSE);
				stdown->getNode()->setType(ELSE);

				// Change ELSE triangle height to a random value between 3 and 10
				float height = rand() % 70 / 10.0f + 3;
				for (int i = 0; i < stdown->_vertices.size(); i++) {
					stdown->_vertices[i] = glm::vec3(stdown->_vertices[i].x, stdown->_vertices[i].y + height, stdown->_vertices[i].z);
				}
			}
		}
	}
}

bool Board::Update(std::vector<Node *> path)
{
	if (path.size() > 1) 
	{
		//Checks if the AI is close to the next node
		glm::vec3 dist = glm::vec3(path[1]->getPosition().x - _AI->Origin.x, 0.0f, path[1]->getPosition().z - _AI->Origin.z);
		if (glm::length(dist) < 1)
		{
			//If it is, update the new AI node and return true
			setAINode(path[1]);
			return true;
		}
		//Otherwise move the AI closer to the node
		else
		{
			dist.x = dist.x / 20;
			dist.z = dist.z / 20;
			_AI->Origin.x += dist.x;
			_AI->Origin.z += dist.z;
			return false;
		}
	}
	
	
}

std::vector <glm::vec3> Board::getAllRoadTriangles()
{
	std::vector<glm::vec3> roadTriangles;
	//Get all tiles
	for (int i = 0; i < _tiles.size(); i++)
	{
		//Get all subSquares(Quadrants)
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		//Get all triangles(halfs)
		for (int j = 0; j < ss.size(); j++)
		{
			//Check if triangle is ROAD, if it is, push its vertices counter-clockwise
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

std::vector <glm::vec3> Board::getAllElseTriangles()
{
	std::vector<glm::vec3> elseTriangles;
	//Get all tiles
	for (int i = 0; i < _tiles.size(); i++)
	{
		//Get all subSquares(Quadrants)
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		//Get all triangles(halfs)
		for (int j = 0; j < ss.size(); j++)
		{
			//Check if triangle is ELSE, if it is, push its vertices counter-clockwise
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			if (stu->getType() == ELSE)
			{
				elseTriangles.push_back(stu->getVertices()[0]);
				elseTriangles.push_back(stu->getVertices()[1]);
				elseTriangles.push_back(stu->getVertices()[2]);
			}
			if (stdown->getType() == ELSE)
			{
				elseTriangles.push_back(stdown->getVertices()[0]);
				elseTriangles.push_back(stdown->getVertices()[1]);
				elseTriangles.push_back(stdown->getVertices()[2]);
			}
		}
	}
	return elseTriangles;
}

std::vector<Node*> Board::getAllNodes()
{
	std::vector<Node*> triangles;
	//Get all tiles
	for (int i = 0; i < _tiles.size(); i++)
	{
		//Get all subSquares(Quadrants)
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		//Get all triangles(halfs)
		for (int j = 0; j < ss.size(); j++)
		{
			//Returns all nodes
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			triangles.push_back(stu->getNode());
			triangles.push_back(stdown->getNode());
		}
	}
	return triangles;
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
