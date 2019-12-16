#include "ParticleSystemPCH.h"
#include "ElapsedTime.h"
#include "PivotCamera.h"
#include "ParticleEffect.h"
#include "CircleGenerator.h"
#include "CrownGenerator.h"
#include "Board.h"

PivotCamera g_Camera;
CircleGenerator g_CircleGeneratorRight;
CircleGenerator g_CircleGeneratorLeft;
CrownGenerator g_CrownGenerator;

#if _DEBUG
ParticleEffect g_ParticleEffect(1000);
#else
ParticleEffect g_ParticleEffectRight(100000);
ParticleEffect g_ParticleEffectLeft(100000);
ParticleEffect g_ParticleEffectHead(1000);
#endif 

int g_iWindowWidth = 1280;
int g_iWindowHeight = 720;
int g_iGLUTWindowHandle = 0;
int g_iErrorCode = 0;

bool g_bLeftMouseDown = false;
bool g_bRightMouseDown = false;

bool g_bUpdate = true;

Board board(100, 20);
glm::vec2 g_MouseCurrent = glm::vec2(0);
glm::vec2 g_MousePrevious = glm::vec2(0);
glm::vec2 g_MouseDelta = glm::vec2(0);

glm::vec3 g_DefaultCameraTranslate( 0, 0, 100 );
glm::vec3 g_DefaultCameraRotate( 40, 0, 0 );
glm::vec3 g_DefaultCameraPivot( 0, 0, 0 );

int g_step = 0;
float g_emitTime = 0;
bool g_right = false;

void InitGL( int argc, char* argv[] );
void DisplayGL();
void IdleGL();
void KeyboardGL( unsigned char c, int x, int y );
void MouseGL( int button, int state, int x, int y );
void MotionGL( int x, int y );
void ReshapeGL( int w, int h );

// We're exiting, cleanup the allocated resources.
void Cleanup();

int main( int argc, char* argv[] )
{
    InitGL( argc, argv );

    g_Camera.SetTranslate( g_DefaultCameraTranslate );
    g_Camera.SetRotate( g_DefaultCameraRotate );
    g_Camera.SetPivot( g_DefaultCameraPivot );
	board.AttachCamera(&g_Camera);
	board.RandomizeTileType();

    if ( g_ParticleEffectRight.LoadTexture( "Data/Textures/Particle-Texture.png" ) 
		&& g_ParticleEffectLeft.LoadTexture("Data/Textures/Particle-Texture.png")
		&& g_ParticleEffectHead.LoadTexture("Data/Textures/snowtexture.png"))
    {
        std::cout << "Successfully loaded particle texture." << std::endl;
    }
    else
    {
        std::cerr << "Failed to load particle texture!" << std::endl;
    }
	g_ParticleEffectRight.SetParticleGenerator(&g_CircleGeneratorRight);
	g_ParticleEffectLeft.SetParticleGenerator(&g_CircleGeneratorLeft);
	g_ParticleEffectHead.SetParticleGenerator(&g_CrownGenerator);
	g_ParticleEffectRight.SetCamera(&g_Camera);
	g_ParticleEffectLeft.SetCamera(&g_Camera);
	g_ParticleEffectHead.SetCamera(&g_Camera);
	g_CrownGenerator.Origin = glm::vec3(0.0f, 20.0f, 10.0f);
	g_ParticleEffectHead.GenerateParticles();
    glutMainLoop();
}

void Cleanup()
{
    if ( g_iGLUTWindowHandle != 0 )
    {
        glutDestroyWindow( g_iGLUTWindowHandle );
        g_iGLUTWindowHandle = 0;
    }
}


void InitGL( int argc, char* argv[] )
{
    std::cout << "Initialise OpenGL..." << std::endl;

    glutInit(&argc, argv);
    int iScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int iScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH );

    glutInitWindowPosition( ( iScreenWidth - g_iWindowWidth ) / 2,
        ( iScreenHeight - g_iWindowHeight ) / 2 );
    glutInitWindowSize( g_iWindowWidth, g_iWindowHeight );

    g_iGLUTWindowHandle = glutCreateWindow( "OpenGL" );

    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutIdleFunc(IdleGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(MotionGL);
    glutKeyboardFunc(KeyboardGL);
    glutReshapeFunc(ReshapeGL);

    // Setup initial GL State
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth( 1.0f );

    glShadeModel( GL_SMOOTH );

    std::cout << "Initialise OpenGL: Success!" << std::endl;
}

void DrawAxis( float fScale, glm::vec3 translate = glm::vec3(0) )
{
    glPushAttrib( GL_ENABLE_BIT );

    glEnable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    glPushMatrix();
    glTranslatef( translate.x, translate.y, translate.z );
    glScalef( fScale, fScale, fScale );
    glColor3f( 0.0f, 0.0f, 1.0f );

    glBegin( GL_LINES );
    {
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.0 );
        glVertex3f( 1.0f, 0.0f, 0.0f );

        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 1.0f, 0.0f );

        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 1.0f );
    }
    glEnd();

    glPopMatrix();
    glPopAttrib();
}

void DisplayGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );                   // Clear the color buffer, and the depth buffer.

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    g_Camera.ApplyViewTransform();
	board.Render();
    DrawAxis( 20.0f, g_Camera.GetPivot() );

    g_ParticleEffectRight.Render();
	g_ParticleEffectLeft.Render();
	g_ParticleEffectHead.Render();

	

    glutSwapBuffers();
    glutPostRedisplay();
}

void IdleGL()
{
    static ElapsedTime elapsedTime;
    float fDeltaTime = elapsedTime.GetElapsedTime();
	g_emitTime += fDeltaTime;

    if ( g_bUpdate )
    {
        g_ParticleEffectRight.Update(fDeltaTime);
		g_ParticleEffectLeft.Update(fDeltaTime);
		g_ParticleEffectHead.Update(fDeltaTime);
    }
    else 
    {
        g_ParticleEffectRight.BuildVertexBuffer();
		g_ParticleEffectLeft.BuildVertexBuffer();
		g_ParticleEffectHead.BuildVertexBuffer();
    }
	g_ParticleEffectHead.AddParticles();
	if (g_CrownGenerator.Origin.x != g_CrownGenerator.Course.x)
	{
		float distance = g_CrownGenerator.Course.x - g_CrownGenerator.Origin.x;
		if (std::abs(distance) < 0.1)
			g_CrownGenerator.Origin.x = g_CrownGenerator.Course.x;
		else
			g_CrownGenerator.Origin.x += 0.1*distance;
	}
    glutPostRedisplay();
}


void KeyboardGL( unsigned char c, int x, int y )
{
    float fRadiusStep = 1.0f;

    switch ( c )
    {
    case ' ': // Space bar
        {
            // Toggle updating of the simulation
            g_bUpdate = !g_bUpdate;
        }
        break;
  
    case 'r':
    case 'R':
        {
            std::cout << "Reset Parameters..." << std::endl;
            g_Camera.SetTranslate( g_DefaultCameraTranslate );
            g_Camera.SetRotate( g_DefaultCameraRotate );
            g_Camera.SetPivot( g_DefaultCameraPivot );
        }
        break;
	case 's':
	case 'S':
	{
		g_step--;
		g_right = !g_right;
		if (g_right) 
		{
			g_CircleGeneratorRight.Origin = glm::vec3(10.0f * g_step, 0.0f, 20.0f);
			g_ParticleEffectRight.GenerateParticles();
		}
		else
		{
			g_CircleGeneratorLeft.Origin = glm::vec3(10.0f * g_step, 0.0f, 0.0f);
			g_ParticleEffectLeft.GenerateParticles();
		}
		g_CrownGenerator.SetCourse(glm::vec3(10.0f * g_step, 0.0f, 0.0f));

	}
	break;
	case 'w':
	case 'W':
	{
		g_step++;
		g_right = !g_right;
		if (g_right)
		{
			g_CircleGeneratorRight.Origin = glm::vec3(10.0f * g_step, 0.0f, 20.0f);
			g_ParticleEffectRight.GenerateParticles();
		}
		else
		{
			g_CircleGeneratorLeft.Origin = glm::vec3(10.0f * g_step, 0.0f, 0.0f);
			g_ParticleEffectLeft.GenerateParticles();
		}
		g_CrownGenerator.SetCourse(glm::vec3(10.0f * g_step, 0.0f, 0.0f));
	}
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

void MouseGL( int button, int state, int x, int y )
{
    if ( button == GLUT_LEFT_BUTTON )
    {
        g_bLeftMouseDown = ( state == GLUT_DOWN );
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        g_bRightMouseDown = ( state == GLUT_DOWN );
    }
    g_MousePrevious = glm::vec2( x, y );

}

void MotionGL( int x, int y )
{
    g_MouseCurrent = glm::vec2( x, y );
    g_MouseDelta = ( g_MousePrevious - g_MouseCurrent );

    // Update the camera
    if ( g_bLeftMouseDown && g_bRightMouseDown )
    {
        g_Camera.TranslateZ( g_MouseDelta.y );
    }
    else if ( g_bLeftMouseDown )
    {
        g_Camera.AddPitch( -g_MouseDelta.y );
        g_Camera.AddYaw( -g_MouseDelta.x );
    }
    else if ( g_bRightMouseDown )
    {
        g_Camera.TranslatePivotX( g_MouseDelta.x );
        g_Camera.TranslatePivotY( -g_MouseDelta.y );
    }    

    g_MousePrevious = g_MouseCurrent;
}

void ReshapeGL( int w, int h )
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    h = std::max( h, 1 );
    
    g_iWindowWidth = w;
    g_iWindowHeight = h;

    g_Camera.SetViewport( 0, 0, w, h );
    g_Camera.ApplyViewport();

    g_Camera.SetProjection( 60.0f, w/(float)h, 0.1f, 1000.0f );
    g_Camera.ApplyProjectionTransform();

    glutPostRedisplay();
}

