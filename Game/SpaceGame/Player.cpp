#include "Player.h"
#include "Pickup.h"
#include "Rocket.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"
#include "Components/PhysicsComponent.h"

FACTORY_REGISTER(Player)

void Player::Update(float dt) {

    // MOVEMENT
    float thrust = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) { 
        thrust = +m_speed; 

        // PARTICLES
        nu::Particle particle;
        particle.position = m_transform.position;
        particle.color = { 1.0f, 0.5f, 0.001f };
        particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
        particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-50.0f, 50.0f) };

        nu::Engine::Get().GetParticleSystem().AddParticle(particle);
    }
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust = -m_speed;

    float rotate = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -1.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = +1.0f;

    auto physicsComponent = GetComponent<nu::PhysicsComponent>();
    if (physicsComponent) {
        nu::Vector2 forward{ 1, 0 };
        nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;
        physicsComponent->ApplyForce(force);
        physicsComponent->ApplyTorque(rotate);

        nu::Vector2 position = physicsComponent->GetPosition();
        position.x = nu::Clamp(0.0f, 1920.0f, position.x);
        position.y = nu::Clamp(0.0f, 1080.0f, position.y);
        physicsComponent->SetPosition(position);
    }


    
    SetRotation(m_transform.rotation + rotate * dt);

    //AddVelocity(velocity * dt);


    Fire();
    
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_X)) {
        nu::Engine::Get().GetTime().SetTimeScale(0.5f);
    }else {
        nu::Engine::Get().GetTime().SetTimeScale(1.0f);
    }

    

    Actor::Update(dt);
}

void Player::SpawnBullet(float offset) {

    auto actor = nu::Factory::Instance().Create<Bullet>("BulletPrototype");
    actor->SetPosition(m_transform.position);
    actor->SetRotation(m_transform.rotation + offset);

    m_scene->AddActor(std::move(actor));
    nu::Engine::Get().GetAudio().PlaySound("laser");
}

void Player::SpawnRocket() {
    auto actor = nu::Factory::Instance().Create<Rocket>("RocketPrototype");
    actor->SetPosition(m_transform.position);
    actor->SetRotation(m_transform.rotation);

    m_scene->AddActor(std::move(actor));
    nu::Engine::Get().GetAudio().PlaySound("laser");
}

void Player::Fire() {
    switch (m_weaponState) {
    case WeaponState::Default:
        m_fireTimer -= nu::Engine::Get().GetTime().GetDeltaTime();
        if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE) && m_fireTimer <= 0.0f) {
            SpawnBullet();
            m_fireTimer = 0.35f;
        }
        break;
    case WeaponState::Rapid:
        m_fireTimer -= nu::Engine::Get().GetTime().GetDeltaTime();
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE) && m_fireTimer <= 0.0f) {
            SpawnBullet();
            m_fireTimer = 0.15f;
        }
        break;
    case WeaponState::Multishot:
        m_fireTimer -= nu::Engine::Get().GetTime().GetDeltaTime();
        if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE) && m_fireTimer <= 0.0f) {
            SpawnBullet();
            SpawnBullet(10.0f);
            SpawnBullet(-10.0f);
            m_fireTimer = 0.35f;
        }
        break;
    case WeaponState::Rocket:
        m_fireTimer -= nu::Engine::Get().GetTime().GetDeltaTime();
        if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE) && m_fireTimer <= 0.0f) {
            SpawnRocket();
            m_fireTimer = 0.85f;
        }
        break;
    default:
        break;
    }
}

void Player::OnCollision(Actor* other) {
    /*if (other->GetTag() == "Enemy") {
        SetDestroyed();
        other->SetDestroyed();
    }*/

    if (other->GetTag() == "Pickup") {
        Pickup* pickup = dynamic_cast<Pickup*>(other);
        SetWeaponState(pickup->GetWeapon());

       other->SetDestroyed(true);
    }
}

void Player::Read(const json::value_t& value){
    Actor::Read(value);
    JSON_READ_NAME(value, "speed", m_speed);
}

std::string Player::WeaponToString()
{
        switch (m_weaponState) {
        case WeaponState::Default: return "Default";
        case WeaponState::Multishot:  return "Multishot";
        case WeaponState::Rapid:   return "Rapid";
        case WeaponState::Rocket:  return "Rocket";
        default:                   return "Default";
        }

}
