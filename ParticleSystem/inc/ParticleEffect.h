#pragma once

#include "Particle.h"

class Camera;
class ParticleGenerator;


class ParticleEffect
{
public:

    // A vertex for the particle
    struct Vertex
    {
        Vertex()
            : m_Pos(0)
            , m_Diffuse(0)
            , m_Tex0(0)
        {}

        glm::vec3   m_Pos;      
        glm::vec4   m_Diffuse; 
        glm::vec2   m_Tex0;   
    };
    ParticleEffect( unsigned int numParticles = 0 );
    virtual ~ParticleEffect();

    void SetCamera( Camera* pCamera );
    void SetParticleGenerator( ParticleGenerator* pGenerator );
    void GenerateParticles();
	void AddParticles(int amount);
	void RemoveDeadParticles();
    virtual void Update( float fDeltaTime );
    virtual void Render();

    bool LoadTexture( const std::string& fileName );
    void Resize( unsigned int numParticles );
	void Clamp(float * f, float * f2);
	float Clamp(float f);

protected:

    void GenerateParticle( Particle& particle );
public:
    
    void BuildVertexBuffer();
private:
    Camera*					_Camera;
    ParticleGenerator*		_ParticleGenerator;
	std::vector<Particle>	_Particles;
	std::vector<Vertex>		_VertexBuffer;
    glm::mat4x4				_LocalToWorldMatrix;
    GLuint					_TextureID;
    glm::vec3				_Gravity;
};