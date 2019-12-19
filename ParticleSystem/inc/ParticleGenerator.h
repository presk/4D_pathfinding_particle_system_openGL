/**
* Resource used:
*	https://www.3dgep.com/simulating-particle-effects-using-opengl/
*/

#pragma once;

#include "Particle.h"

class ParticleGenerator
{
public:
    virtual ~ParticleGenerator() {}
    virtual void GenerateParticle( Particle& particle ) = 0;

};