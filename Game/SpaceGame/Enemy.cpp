#include "Enemy.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Components/PhysicsComponent.h"

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt) {
    Player* player = m_scene->GetActorByTag<Player>("Player");
    if (player) {
        nu::Vector2 direction = player->GetTransform().position - m_transform.position;
        float rotation = direction.Angle();
        SetRotation(rotation * nu::RadToDeg);

        auto physicsComponent = GetComponent<nu::PhysicsComponent>();
        if (physicsComponent) {
            nu::Vector2 forward{ 1, 0 };
            nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;

            physicsComponent->ApplyForce(force);

            nu::Vector2 direction = player->GetTransform().position - m_transform.position;
            float rotation = direction.Angle();
            physicsComponent->SetRotation(rotation * nu::RadToDeg);

            nu::Vector2 position = physicsComponent->GetPosition();
            position.x = nu::Clamp(0.0f, 1920.0f, position.x);
            position.y = nu::Clamp(0.0f, 1080.0f, position.y);
            physicsComponent->SetPosition(position);
        }
    }
        
    //AddVelocity(velocity * dt);

    Actor::Update(dt);
}

void Enemy::Read(const json::value_t& value) {
    Actor::Read(value);
    /*JSON_READ_NAME(value, "points", );
    JSON_READ_NAME(value, "health", );*/
    JSON_READ_NAME(value, "speed", m_speed);
}

void Enemy::OnCollision(Actor* other) {
    if (other->GetTag() == "Bullet" || other->GetTag() == "Rocket") {
        SetDestroyed();
        other->SetDestroyed();

        ((SpaceGame*)m_scene->GetGame())->AddPoints(100);

        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            particle.color = { nu::RandomColor(), nu::RandomColor(), nu::RandomColor() };
            particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
            particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

            nu::Engine::Get().GetParticleSystem().AddParticle(particle);
        }
    }
}
