#include "pch.h"
#include "ParticleSystem.h"
#include "Renderer.h"

using namespace nu;

bool ParticleSystem::Initialize(size_t size) {
	// reserve # particles in container
	m_particles.resize(size);

	return true;
}

void ParticleSystem::Shutdown() {
	// clear all particles
	m_particles.clear();
}

void ParticleSystem::Update(float dt) {
	// update active particles
	for (auto& particle : m_particles) {
		if (!particle.active) continue;

		// reduce particle.lifespan by subtracting delta time
		particle.lifespan -= dt;

		// set active if (particle.lifespan > 0);
		particle.active = particle.lifespan > 0;
		// update position with velocity (multiply by dt)
		particle.position += particle.velocity * dt;
	}
}

void ParticleSystem::Draw(const Renderer& renderer)
{
	// draw all active particlee
	for (auto& particle : m_particles)
	{
		if (particle.active)
		{
			// set particle color and draw point at current position
			renderer.SetColor(particle.color, 1.0f);
			renderer.DrawFillRect(particle.position.x, particle.position.y, 3.5f, 3.5f);
		}
	}
}

void ParticleSystem::AddParticle(const Particle& particle) {
	// get free particle
	Particle* freeParticle = GetFreeParticle();
	// check if free particle is not nullptr
	if (freeParticle) {
		// set free particle object with particle
		*freeParticle = particle;
		// set particle active
		freeParticle->active = true;
	}
}

Particle* ParticleSystem::GetFreeParticle() {
	for (auto& particle : m_particles) {
		if (!particle.active)
			return &particle;
	}

	return nullptr;
}