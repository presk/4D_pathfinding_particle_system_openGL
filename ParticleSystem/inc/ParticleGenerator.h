#pragma once;

#include "Particle.h"

class ParticleGenerator
{
public:
    virtual ~ParticleGenerator() {}
    virtual void GenerateParticle( Particle& particle ) = 0;

};