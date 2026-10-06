#pragma once
#include "Framework/Actor.h"

class Enemy : public nu::Actor {
public:
    Enemy() = default;

    CLASS_PROTOTYPE(Enemy)

    void Update(float dt) override;
    void Read(const json::value_t& value) override;
    void OnCollision(Actor* other) override;

private:
    int m_ammo = 0;
    float m_speed = 800.0f;
};