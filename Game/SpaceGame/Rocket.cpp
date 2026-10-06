#include "Rocket.h"
#include "Framework/Scene.h"
#include "Core/Factory.h"
#include "Pellet.h"

FACTORY_REGISTER(Rocket)

void Rocket::Read(const json::value_t& value) {
    Bullet::Read(value);

    JSON_READ_NAME(value, "pelletamount", m_pelletAmount);
}

void Rocket::Destroy() {
    Explode();

    Actor::Destroy();
}

void Rocket::Explode() {

    float angle = 360.0f / static_cast<float>(m_pelletAmount);

    for (int i = 0; i < m_pelletAmount; ++i) {
        auto actor = nu::Factory::Instance().Create<Pellet>("PelletPrototype");
        actor->SetPosition(m_transform.position);
        actor->SetRotation(i * angle);

        m_scene->AddActor(std::move(actor));
    }
}
