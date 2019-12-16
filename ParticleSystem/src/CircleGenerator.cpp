#include "ParticleSystemPCH.h"
#include "Random.h"
#include "CircleGenerator.h"

CircleGenerator::CircleGenerator()
{
	//Sets a limited radius similar to a doughnut
	MinRadius = 3.9;
	MaxRadius = 4;
	MinAz = 0;
	MaxAz = 90;
	MinPolar = 0;
	MaxPolar = 360;
	MinSpeed = 5;
	MaxSpeed = 5;
	MinLifetime = 1;
	MaxLifetime = 2;
	Origin = glm::vec3(0,0,0);
}
//Generates a particle using random bounded values
void CircleGenerator::GenerateParticle(Particle & particle)
{
	//Sets a random value between the bounds for the polar and azimuth angles, 
	//the radius, speed and lifetime of the particle
	float az = glm::radians(RandRange(MinAz, MaxAz));
	float pol = glm::radians(RandRange(MinPolar, MaxPolar));
	float radius = RandRange(MinRadius, MaxRadius);
	float speed = RandRange(MinSpeed, MaxSpeed);
	float lifetime = RandRange(MinLifetime, MaxLifetime);

	//Converts spherical coords into i j k coords
	float X = cosf(pol) * radius;
	float Y = 0.1;
	float Z = sinf(pol) * radius;

	glm::vec3 positionVector(X, Y, Z);
	Y = sinf(az) * speed;
	glm::vec3 speedVector(X, Y, Z);

	particle.m_Position = positionVector + Origin;
	particle.m_Velocity = speedVector;
	particle.m_fLifeTime = lifetime;
	particle.m_fAge = 0;
}

void CircleGenerator::RenderCircle(glm::vec4 color, float radius)
{
	float X, Y, Z, polA;
	glColor4fv(glm::value_ptr(color));
	glPointSize(2.0f);
	glBegin(GL_POINTS);

	for (float pol = MinPolar; pol < MaxPolar; pol += 5.0f)
	{
		polA = glm::radians(pol);
		X = radius * cosf(polA);
		Y = radius * sinf(polA);
		Z = radius;
		glVertex3f(X, Y, Z);
	}
	polA = glm::radians(MaxPolar);
	X = MaxRadius * cosf(polA);
	Y = MaxRadius * sinf(polA);
	Z = MaxRadius;

	glVertex3f(X, Y, Z);
	glEnd();
}
