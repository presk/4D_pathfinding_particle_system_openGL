#include "ParticleSystemPCH.h"
#include "obstacles.h"



ObstacleSet::ObstacleSet()
{
}

ObstacleSet::ObstacleSet(int boardLength, int count, PivotCamera * cam) : boardLength(boardLength)
{
	_camera = cam;
	// Fill the set
	for (int o = 0; o < count; o++) {
		obstacles.push_back(Hypercube());
	}

	// Randomize the set
	randomize();
}

void ObstacleSet::Render()
{
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glm::vec3 translate = _camera->GetPivot();
	glTranslatef(translate.x, translate.y, translate.z);
	//glScalef(fScale, fScale, fScale);
	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);

	glColor4f(0.25f, 0.25f, 0.25f, 1.0f);

	std::vector<glm::vec3> verts = getObstacleVertices();

	for (int v = 0; v < verts.size(); v++) {
		glVertex3f(verts[v].x, verts[v].y, verts[v].z);
	}

	glEnd();

	glPopMatrix();
	glPopAttrib();
}

void ObstacleSet::randomize()
{
	for (int o = 0; o < obstacles.size(); o++) {
		obstacles[o].setPos4D(rand() % 50 / 10.0f - 2.5f); // -2.5 to 2.5
		obstacles[o].setRotation4D(rand() % 360, XW); // 0 to 360
		obstacles[o].setRotation4D(rand() % 360, YW); // 0 to 360
		obstacles[o].setRotation4D(rand() % 360, ZW); // 0 to 360
		obstacles[o].setPosition(glm::vec3(rand() % boardLength * 10 / 10.0f - boardLength / 2.0f, 0.0f, rand() % boardLength * 10 / 10.0f - boardLength / 2.0f)); // -length/2 to length/2
		obstacles[o].setScale(glm::vec3(rand() % 25 / 10.0f + 2.5f, rand() % 50 / 10.0f + 5, rand() % 25 / 10.0f + 2.5f)); // 2.5 to 5.0, 5.0 to 10.0, 2.5 to 5.0
	}
}

bool ObstacleSet::testNode(glm::vec3 nodeCoords)
{
	for (int o = 0; o < obstacles.size(); o++) {
		if (testDOP18(obstacles[o].getDOP18(), nodeCoords)) return true;
	}

	return false;
}

vector<glm::vec3> ObstacleSet::getObstacleVertices()
{
	vector<glm::vec3> out;

	for (int o = 0; o < obstacles.size(); o++) {
		vector<glm::vec3> oVerts = obstacles[o].getVerts();

		for (int v = 0; v < oVerts.size(); v++) {
			out.push_back(oVerts[v]);
		}
	}

	return out;
}

