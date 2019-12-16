#include "ParticleSystemPCH.h"
#include "ParticleGenerator.h"
#include "Camera.h"
#include "Random.h"
#include "ParticleEffect.h"

ParticleEffect::ParticleEffect( unsigned int numParticles /* = 0 */ )
: _Camera( NULL )
, _ParticleGenerator( NULL )
, _LocalToWorldMatrix(1)
, _TextureID(0)
, _Gravity( 0, -15.0f, 0 ) 
{
    Resize(numParticles);
}

ParticleEffect::~ParticleEffect()
{
    if ( _TextureID != 0 )
    {
        glDeleteTextures( 1, &_TextureID );
        _TextureID = 0;
    }
}

//Gives access to the camera
void ParticleEffect::SetCamera( Camera* pCamera )
{
    _Camera = pCamera;
}

//Selects the type of particle generator
void ParticleEffect::SetParticleGenerator( ParticleGenerator* pGenerator )
{
    _ParticleGenerator = pGenerator;
}

//Uses SOIL to load textures for the particles
bool ParticleEffect::LoadTexture( const std::string& fileName )
{
    if ( _TextureID != 0 )
    {
        glDeleteTextures(1, &_TextureID );
    }

    _TextureID = SOIL_load_OGL_texture( fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS );

    return ( _TextureID != 0 );
}


//Calls the right Generator for the particle
void ParticleEffect::GenerateParticle( Particle& particle )
{
    _ParticleGenerator->GenerateParticle( particle );
}

//Generates particles
void ParticleEffect::GenerateParticles()
{
    if ( _ParticleGenerator != NULL )
    {
        for ( unsigned int i = 0; i < _Particles.size(); ++i )
        {
            GenerateParticle(_Particles[i]);
        }
    }
}
//Adds more particles
void ParticleEffect::AddParticles()
{
	for (int i = 0; i < 500; i++)
	{
		Particle particle = Particle();
		GenerateParticle(particle);
		_Particles.push_back(particle);
	}
	
}

//Removes all the particles that have touched the ground
void ParticleEffect::RemoveDeadParticles()
{

	for (std::vector<Particle>::iterator iter = _Particles.begin(); iter != _Particles.end(); ++iter)
	{
		if (iter->m_Position.y <= 0)
		{
			_Particles.erase(iter);
			break;
		}
	}
}


void ParticleEffect::BuildVertexBuffer()
{
    const glm::vec3 X( 0.5, 0, 0 );
    const glm::vec3 Y( 0, 0.5, 0 );
    const glm::vec3 Z( 0, 0 ,1.0 );

    glm::quat cameraRotation;

    if ( _Camera != NULL )
    {
        cameraRotation = glm::quat( glm::radians(_Camera->GetRotation()) );    
    }

    _VertexBuffer.resize(_Particles.size() * 4, Vertex() );

    for ( unsigned int i = 0; i < _Particles.size(); ++i )
    {
        Particle& particle = _Particles[i];
        glm::quat rotation = glm::angleAxis( particle.m_fRotate, Z );

        unsigned int vertexIndex = i * 4;
        Vertex& v0 = _VertexBuffer[vertexIndex + 0];   // Bottom-left
        Vertex& v1 = _VertexBuffer[vertexIndex + 1];   // Bottom-right
        Vertex& v2 = _VertexBuffer[vertexIndex + 2];   // Top-right
        Vertex& v3 = _VertexBuffer[vertexIndex + 3];   // Top-left

        // Bottom-left
        v0.m_Pos = particle.m_Position + ( rotation * ( -X - Y ) * particle.m_fSize ) * cameraRotation;
        v0.m_Tex0 = glm::vec2( 0, 1 );
        v0.m_Diffuse = particle.m_Color;

        // Bottom-right
        v1.m_Pos = particle.m_Position + ( rotation * ( X - Y ) * particle.m_fSize ) * cameraRotation;
        v1.m_Tex0 = glm::vec2( 1, 1 );
        v1.m_Diffuse = particle.m_Color;

        // Top-right
        v2.m_Pos = particle.m_Position + ( rotation * ( X + Y ) * particle.m_fSize ) * cameraRotation;
        v2.m_Tex0 = glm::vec2( 1, 0 );
        v2.m_Diffuse = particle.m_Color;

        // Top-left
        v3.m_Pos = particle.m_Position + ( rotation * ( -X + Y ) * particle.m_fSize ) * cameraRotation;
        v3.m_Tex0 = glm::vec2( 0, 0 );
        v3.m_Diffuse = particle.m_Color;
    }
    
}

void ParticleEffect::Update(float DeltaTime)
{
    for ( unsigned int i = 0; i < _Particles.size(); ++i )
    {
        Particle& particle = _Particles[i];
        particle.m_fAge += DeltaTime;
        float lifeRatio = glm::saturate(particle.m_fAge / particle.m_fLifeTime);
        particle.m_Velocity += ( _Gravity * DeltaTime );

		//Stops the particules from going underground
		if (particle.m_Position.y <= 0) { particle.m_Position.y = 0; }
		else { particle.m_Position += (particle.m_Velocity * DeltaTime); }
		glm::vec3 pCDist = particle.m_Position;
		pCDist.y = 0.0f;
		//Sets a color depending on where the particule is
		particle.m_Color = glm::vec4(acosf(pCDist.x/glm::length(pCDist)), 0.05f * particle.m_Position.y, asinf(pCDist.z / glm::length(pCDist)), 1);//m_ColorInterpolator.GetValue( lifeRatio );
		//Rotates the particules as they move
		particle.m_fRotate = glm::lerp<float>( 0.0f, 720.0f, lifeRatio );

        particle.m_fSize = glm::lerp<float>( 0.8f, 0.0f, lifeRatio);
    }
	RemoveDeadParticles();
    BuildVertexBuffer();
}



void ParticleEffect::Render()
{
    glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
    glEnable(GL_BLEND);                 // Enable Blending
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
    glEnable(GL_TEXTURE_2D);            // Enable textures

    glPushMatrix();
    glMultMatrixf( glm::value_ptr(_LocalToWorldMatrix) );
    
    glBindTexture( GL_TEXTURE_2D, _TextureID );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &(_VertexBuffer[0].m_Pos) );
    glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &(_VertexBuffer[0].m_Tex0) );
    glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &(_VertexBuffer[0].m_Diffuse) );

    glDrawArrays( GL_QUADS, 0, _VertexBuffer.size() );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glBindTexture( GL_TEXTURE_2D, 0 );


    glPopMatrix();

}

void ParticleEffect::Resize( unsigned int numParticles )
{
    _Particles.resize( numParticles,  Particle() );
    _VertexBuffer.resize( numParticles * 4, Vertex() );
}
