#pragma once

#include "ParticleGenerator.h"

//Generator for a circular impact particle system
class CircleGenerator : public ParticleGenerator
{
public:
	CircleGenerator();
	virtual void GenerateParticle(Particle& particle);

	float MinRadius;
	float MaxRadius;
	float MinAz;
	float MaxAz;
	float MinPolar;
	float MaxPolar;
	float MinSpeed;
	float MaxSpeed;
	float MinLifetime;
	float MaxLifetime;
	glm::vec3   Origin;

private:
	void RenderCircle(glm::vec4 color, float fRadius);
};
