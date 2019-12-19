// Author: Alexia Soucy, 40014822
// COMP477 F19, Team 8
// Container for randomly generated 4D obstacles and their bounding volumes.

#pragma once
/*// OpenGL functionality
#include <GL\glew.h>
#include <GL\freeglut.h>*/

// Linear algebra
#include "ParticleSystemPCH.h"
using namespace glm;

// Used for vertex storage
#include <vector>
using namespace std;

// Collision detection and 4D
#include "gl4d.h"
#include "dop18.h"
#include "ParticleSystemPCH.h"
#include "PivotCamera.h"

// A set of 4D obstacles, generated automatically. Their 3D cross-section can be recalculated on command whenever the dimensional threshold changes through user input.
class ObstacleSet {

private:
	// The set of obstacles
	vector<Hyperobject> obstacles;

	// The current set of vertices
	vector<vec3> vertices;

	// The length of the board
	int boardLength;
	PivotCamera * _camera;

	// Wireframe toggle for rendering
	bool wireframe;

public:
	ObstacleSet() {};

	ObstacleSet(int boardLength, int count, PivotCamera * cam) : boardLength(boardLength) {
		wireframe = false;
		_camera = cam;
		// Fill the set
		for (int o = 0; o < count; o++) {
			obstacles.push_back(Hypercube());
		}

		// Randomize the set
		randomize();
	}

	// Renders the 4D obstacles with or without wireframe. Vertex colours scale based on vertex position.
	void Render()
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

		for (int v = 0; v < vertices.size(); v++) {

			// Colour vertices based on their world space coordinates
			glColor4f((vertices[v].x + 50.0f) / 100.0f, (vertices[v].y + 50.0f) / 100.0f, (vertices[v].z + 50.0f) / 100.0f, 1.0f);
			glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
		}

		glEnd();

		if (wireframe) {
			glBegin(GL_LINES);

			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

			for (int v = 0; v < vertices.size(); v+=3) {
				// Draw triangle's edges
				glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
				glVertex3f(vertices[v + 1].x, vertices[v + 1].y, vertices[v + 1].z);

				glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
				glVertex3f(vertices[v + 2].x, vertices[v + 2].y, vertices[v + 2].z);

				glVertex3f(vertices[v + 1].x, vertices[v + 1].y, vertices[v + 1].z);
				glVertex3f(vertices[v + 2].x, vertices[v + 2].y, vertices[v + 2].z);
			}

			glEnd();
		}

		glPopMatrix();
		glPopAttrib();
	}

	// Randomize all obstacles' position, scale, rotation, and 4D offset.
	void randomize() {
		for (int o = 0; o < obstacles.size(); o++) {
			obstacles[o].setPos4D(rand() % 50 / 10.0f - 2.5f); // -2.5 to 2.5
			obstacles[o].setRotation4D(rand() % 360, XW); // 0 to 360
			obstacles[o].setRotation4D(rand() % 360, YW); // 0 to 360
			obstacles[o].setRotation4D(rand() % 360, ZW); // 0 to 360
			obstacles[o].setPosition(glm::vec3(rand() % boardLength * 10 / 10.0f - boardLength / 2.0f, 0.0f, rand() % boardLength * 10 / 10.0f - boardLength / 2.0f)); // -length/2 to length/2
			obstacles[o].setScale(glm::vec3(rand() % 25 / 10.0f + 2.5f, rand() % 50 / 10.0f + 5, rand() % 25 / 10.0f + 2.5f)); // 2.5 to 5.0, 5.0 to 10.0, 2.5 to 5.0
		}

		// Get the set of vertices to render
		computeCross();
	}
	// Checks a node's coordinates against all obstacles; returns true if obstructed
	bool testNode(glm::vec3 nodeCoords) {
		for (int o = 0; o < obstacles.size(); o++) {
			if (testDOP18(obstacles[o].getDOP18(), nodeCoords)) return true;
		}

		return false;
	}

	// Processes obstacle vertices in order to render as triangles
	void computeCross() {
		// Delete all old vertices
		vertices.clear();

		// Compute the cross-section of all vertices for all obstacles
		for (int o = 0; o < obstacles.size(); o++) {
			vector<glm::vec3> oVerts = obstacles[o].getVerts();

			for (int v = 0; v < oVerts.size(); v++) {
				vertices.push_back(oVerts[v]);
			}
		}
	}

	// Toggles the wireframe to display obstacle edges
	void toggleWireframe() {
		wireframe = !wireframe;
	}
};