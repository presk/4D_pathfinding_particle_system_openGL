#pragma once
#include "ParticleGenerator.h"

//Generator for a circular crown particle system shooting up
class LampGenerator : public ParticleGenerator
{
public:
	LampGenerator();
	virtual void GenerateParticle(Particle& particle);

	void SetCourse(glm::vec3 course);

	float MinRadius;
	float MaxRadius;
	float MinPolar;
	float MaxPolar;
	float MinSpeed;
	float MaxSpeed;
	float MinLifetime;
	float MaxLifetime;
	glm::vec3 Origin;
	glm::vec3 Course;

private:
	void RenderLamp(glm::vec4 color, float fRadius);
};
