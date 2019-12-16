#include "ParticleSystemPCH.h"
#include "Board.h"
#include "..\inc\Board.h"

Board::Board(int bSize, int tCount)
{
	//std::cout << "Board Creation Success!" << std::endl;
	float tSize = (float)bSize / (float)tCount;
	for (int i = 0; i < tCount; i++)
	{
		for (int j = 0; j < tCount; j++)
		{
			Tile * t = new Tile(glm::vec3(tSize/2.0f - bSize/2.0f + i*tSize, 0, tSize / 2.0f - bSize / 2.0f + j*tSize), tSize);
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
	glColor3f(0.0f, 0.0f, 1.0f);
	std::vector<glm::vec3> roadTriangles = getAllRoadTriangles();

	glBegin(GL_TRIANGLES);
	{
		for (int i = 0; i < roadTriangles.size(); i++)
		{
			glVertex3f(roadTriangles[i].x, roadTriangles[i].y, roadTriangles[i].z);
		}
	}
	glEnd();

	glPopMatrix();
	glPopAttrib();
}

void Board::AttachCamera(PivotCamera * cam)
{
	_camera = cam;
}

void Board::RandomizeTileType()
{
	srand(time(NULL));
	std::vector<glm::vec3> roadTriangles;
	for (int i = 0; i < _tiles.size(); i++)
	{
		std::vector<SubSquare*> ss = _tiles[i]->getAllSquares();
		for (int j = 0; j < ss.size(); j++)
		{
			SubTriangle* stu = ss[j]->getUpTriangle();
			SubTriangle* stdown = ss[j]->getDownTriangle();
			if (rand() % 10  < 7)
			{
				stu->setType(ELSE);
			}
			if (rand() % 10  < 7)
			{
				stdown->setType(ELSE);
			}
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
