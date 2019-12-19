#include "ParticleSystemPCH.h"
#include "Random.h"
#include "CrownGenerator.h"

CrownGenerator::CrownGenerator()
{
	//Creates a cylinder prism using cylindrical coordinates
	MinRadius = 0;
	MaxRadius = 2;
	MinPolar = 0;
	MaxPolar = 360;
	MinSpeed = 0.5;
	MaxSpeed = 15;
	MinLifetime = 0.1;
	MaxLifetime = 2;
	Origin = glm::vec3(0, 0, 0);
	Course = Origin;
}

//Generates a particle using random bounded values
void CrownGenerator::GenerateParticle(Particle & particle)
{
	//Sets a random value between the bounds for the polar angle, 
	//the radius, speed and lifetime of the particle
	float Polar = glm::radians(RandRange(MinPolar, MaxPolar));
	float radius = RandRange(MinRadius, MaxRadius);
	float speed = RandRange(MinSpeed, MaxSpeed);
	float lifetime = RandRange(MinLifetime, MaxLifetime);

	//Converts cylindrical coords into i j k coords
	float X = cosf(Polar) * radius;
	float Y = 0.1;
	float Z = sinf(Polar) * radius;
	glm::vec3 positionVector(X, Y, Z);
	X = 0;
	Y = -speed; //speed
	Z = 0;
	glm::vec3 speedVector(X, Y, Z);
	particle.m_Position = positionVector + Origin;
	particle.m_Velocity = speedVector;
	particle.m_fLifeTime = lifetime;
	particle.m_fAge = 0;
}

//Defines the destination point after a movement
void CrownGenerator::SetCourse(glm::vec3 course)
{
	Course = course;
}

void CrownGenerator::RenderCrown(glm::vec4 color, float fRadius)
{
	float X, Y, Z, azi;
	glColor4fv(glm::value_ptr(color));
	glPointSize(2.0f);
	glBegin(GL_POINTS);

	for (float Polar = MinPolar; Polar < MaxPolar; Polar += 5.0f)
	{
		azi = glm::radians(Polar);
		X = fRadius * cosf(azi);
		Y = fRadius * sinf(azi);
		Z = fRadius;
		glVertex3f(X, Y, Z);
	}
	azi = glm::radians(MaxPolar);
	X = MaxRadius * cosf(azi);
	Y = MaxRadius * sinf(azi);
	Z = MaxRadius;
	glVertex3f(X, Y, Z);
	glEnd();
}
