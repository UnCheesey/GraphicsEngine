#include "Pellet.h"
#include "Math/MathUtils.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"

FACTORY_REGISTER(Pellet)

void Pellet::Update(float dt) {
    auto physicsComponent = GetComponent<nu::PhysicsComponent>();
    if (physicsComponent) {
        nu::Vector2 forward{ 1, 0 };
        nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;
        physicsComponent->SetVelocity(velocity);

        nu::Vector2 position = physicsComponent->GetPosition();
        position.x = nu::Wrap(0.0f, 1920.0f, position.x);
        position.y = nu::Wrap(0.0f, 1080.0f, position.y);
        physicsComponent->SetPosition(position);
    }

    Actor::Update(dt);
}

void Pellet::Read(const json::value_t& value) {
	Bullet::Read(value);
};