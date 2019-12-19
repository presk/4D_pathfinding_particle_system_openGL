/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	Alexia Soucy, 40014822
*	Maude Braunstein, 27545967
* 
* Team:
*	COMP477 F19, Team 8
*
*/

#include "ParticleSystemPCH.h"
#include "ElapsedTime.h"
#include "PivotCamera.h"
#include "ParticleEffect.h"
#include "CrownGenerator.h"
#include "Board.h"
#include "Graph.h"
#include "obstacles.h"


//Global Variables
PivotCamera g_Camera;
CrownGenerator g_CrownGenerator;
ParticleEffect g_ParticleEffectHead(100);


int g_iWindowWidth = 1280;
int g_iWindowHeight = 720;
int g_iGLUTWindowHandle = 0;
int g_iErrorCode = 0;

bool g_bUpdate = true;

Board board(100, 10);
Graph graph;
ObstacleSet obstacles;


// Camera presets
glm::vec3 g_DefaultCameraTranslate(0, 0, 100);
glm::vec3 g_DefaultCameraRotate(40, 0, 0);
glm::vec3 g_DefaultCameraPivot(0, 0, 0);

glm::vec3 g_OverheadCameraRotate(90, 0, 0);

// Pseudo-Eyelevel
glm::vec3 g_EyeLevelCameraTranslate(0, 0, 100);
glm::vec3 g_EyeLevelCameraRotate(10, 0, 0);

float g_emitTime = 0;

void InitGL(int argc, char* argv[]);
void DisplayGL();
void IdleGL();
void KeyboardGL(unsigned char c, int x, int y);
void MouseGL(int button, int state, int x, int y);
void CameraGL(int key, int x, int y);
void ReshapeGL(int w, int h);

// We're exiting, cleanup the allocated resources.
void Cleanup();

int main(int argc, char* argv[])
{
	srand(time(NULL));
	InitGL(argc, argv);

	g_Camera.SetTranslate(g_DefaultCameraTranslate);
	g_Camera.SetRotate(g_DefaultCameraRotate);
	g_Camera.SetPivot(g_DefaultCameraPivot);

	//Attach Camera and Particle Effect(AI) and initialize the board
	board.AttachCamera(&g_Camera);
	board.AttachAI(&g_CrownGenerator);
	board.RandomizeTileType();
	//Initializes the overlaying graph and calculates the initial A* path
	graph = Graph(&board, &g_Camera);
	//Initializes the obstacles
	obstacles = ObstacleSet(100, 50, &g_Camera);
	//Sets the proper obstruction flags
	std::vector<Node *> n = board.getAllNodes();
	for (auto node : n)
	{
		if (obstacles.testNode(node->getPosition()))
		{
			node->setObstructed(true);
		}
		else
		{
			node->setObstructed(false);
		}
	}


	if ( g_ParticleEffectHead.LoadTexture("Data/Textures/ctext.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}
	g_ParticleEffectHead.SetParticleGenerator(&g_CrownGenerator);
	g_ParticleEffectHead.SetCamera(&g_Camera);
	g_ParticleEffectHead.GenerateParticles();

	glutMainLoop();
}

void Cleanup()
{
	if (g_iGLUTWindowHandle != 0)
	{
		glutDestroyWindow(g_iGLUTWindowHandle);
		g_iGLUTWindowHandle = 0;
	}
}


void InitGL(int argc, char* argv[])
{
	std::cout << "Initialise OpenGL..." << std::endl;

	glutInit(&argc, argv);
	int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition((iScreenWidth - g_iWindowWidth) / 2,
		(iScreenHeight - g_iWindowHeight) / 2);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);

	g_iGLUTWindowHandle = glutCreateWindow("OpenGL");

	// Register GLUT callbacks
	glutDisplayFunc(DisplayGL);
	glutIdleFunc(IdleGL);
	glutMouseFunc(MouseGL);
	glutKeyboardFunc(KeyboardGL);
	glutReshapeFunc(ReshapeGL);
	glutSpecialFunc(CameraGL);

	// Setup initial GL State
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);

	std::cout << "Initialise OpenGL: Success!" << std::endl;

	/*// Instruct user.
	std::cout <<
		"Change Camera Position:" << endl <<
		"   Press F1 for default camera view, looking out at the board angled above" << endl <<
		"   Press F2 for a completely overhead view of the board" << endl <<
		"   Press F3 for a near-eye level view of the board" << endl <<
		"Rotate the camera around with the arrow keys" << endl;*/

}


void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // Clear the color buffer, and the depth buffer.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	g_Camera.ApplyViewTransform();
	//Render all the objects
	board.Render();
	graph.Render();
	obstacles.Render();
	g_ParticleEffectHead.Render();



	glutSwapBuffers();
	glutPostRedisplay();
}

void IdleGL()
{
	static ElapsedTime elapsedTime;
	float fDeltaTime = elapsedTime.GetElapsedTime();
	g_emitTime += fDeltaTime;

	if (g_bUpdate)
	{
		g_ParticleEffectHead.Update(fDeltaTime);
		graph.Update();

		if (graph._targetReached)
		{
			{
				// Cleanup up and quit
				exit(0);
			}
		}

	}
	else
	{
		g_ParticleEffectHead.BuildVertexBuffer();
	}
	g_ParticleEffectHead.AddParticles(100);

	glutPostRedisplay();
}


void KeyboardGL(unsigned char c, int x, int y)
{
	float fRadiusStep = 1.0f;

	switch (c)
	{
	case ' ': // Space bar
	{
		// Toggle updating of the simulation
		g_bUpdate = !g_bUpdate;
	}
	break;
	case 'f':
	case 'F':
		// Toggle wireframe
		obstacles.toggleWireframe();
		break;
	case '\033': // escape quits
	case 'Q':    // Q quits
	case 'q':    // q (or escape) quits
	{
		// Cleanup up and quit
		exit(0);
	}
	break;
	}

	glutPostRedisplay();
}

void MouseGL(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && Hyperobject::threshold < 2.5f) {
		Hyperobject::setDimensionalThreshold(Hyperobject::threshold + 0.1f);
		obstacles.computeCross();
		std::vector<Node *> n = board.getAllNodes();
		for (auto node : n)
		{
			if (obstacles.testNode(node->getPosition()))
			{
				node->setObstructed(true);
			}
			else
			{
				node->setObstructed(false);
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && Hyperobject::threshold > -2.5f) {
		Hyperobject::setDimensionalThreshold(Hyperobject::threshold - 0.1f);
		obstacles.computeCross();
		std::vector<Node *> n = board.getAllNodes();
		for (auto node : n)
		{
			if (obstacles.testNode(node->getPosition()))
			{
				node->setObstructed(true);
			}
			else
			{
				node->setObstructed(false);
			}
		}
	}

}

void ReshapeGL(int w, int h)
{
	std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

	h = std::max(h, 1);

	g_iWindowWidth = w;
	g_iWindowHeight = h;

	g_Camera.SetViewport(0, 0, w, h);
	g_Camera.ApplyViewport();

	g_Camera.SetProjection(60.0f, w / (float)h, 0.1f, 1000.0f);
	g_Camera.ApplyProjectionTransform();

	glutPostRedisplay();
}

void CameraGL(int key, int x, int y)
{
	if (key == GLUT_KEY_F1) {
		std::cout << "Default Camera" << endl;
		g_Camera.SetTranslate(g_DefaultCameraTranslate);
		g_Camera.SetRotate(g_DefaultCameraRotate);
		g_Camera.SetPivot(g_DefaultCameraPivot);
	}
	if (key == GLUT_KEY_F2) {
		std::cout << "Overhead Camera" << endl;
		g_Camera.SetTranslate(g_DefaultCameraTranslate);
		g_Camera.SetRotate(g_OverheadCameraRotate);
		g_Camera.SetPivot(g_DefaultCameraPivot);
	}
	if (key == GLUT_KEY_F3) {
		std::cout << "Eye Level Camera" << endl;
		g_Camera.SetTranslate(g_EyeLevelCameraTranslate);
		g_Camera.SetRotate(g_EyeLevelCameraRotate);
		g_Camera.SetPivot(g_DefaultCameraPivot);
	}
	if (key == GLUT_KEY_LEFT) {
		glm::vec3 cur = g_Camera.GetRotation();
		cur.y += 5.0f;
		g_Camera.SetRotate(cur);
	}
	if (key == GLUT_KEY_RIGHT) {
		glm::vec3 cur = g_Camera.GetRotation();
		cur.y -= 5.0f;
		g_Camera.SetRotate(cur);
	}
	if (key == GLUT_KEY_UP) {
		glm::vec3 cur = g_Camera.GetRotation();
		cur.x += 5.0f;
		g_Camera.SetRotate(cur);
	}
	if (key == GLUT_KEY_DOWN) {
		glm::vec3 cur = g_Camera.GetRotation();
		cur.x -= 5.0f;
		g_Camera.SetRotate(cur);
	}

	glutPostRedisplay();
}
