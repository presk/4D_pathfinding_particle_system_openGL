/**
* Authors:
*	Keven Presseau-St-Laurent, 40000501
*	
*	
*
* Team:
*	COMP477 F19, Team 8

* Crown Generator is a ParticleEffect generator that generates particule from
* a small cylindrical prism
*
* Resource used:
*	https://www.3dgep.com/simulating-particle-effects-using-opengl/
*/
#pragma once

#include "ParticleGenerator.h"


class CrownGenerator : public ParticleGenerator
{
public:
	CrownGenerator();
	virtual void GenerateParticle(Particle& particle);

	void SetCourse(glm::vec3 course);					//Defines the destination point after a movement

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
	void RenderCrown(glm::vec4 color, float fRadius);
};
