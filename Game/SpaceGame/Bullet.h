#pragma once
#include "Framework/Actor.h"

class Bullet : public nu::Actor {
public:
    Bullet() = default;

    CLASS_PROTOTYPE(Bullet)

    void Update(float dt) override;

    void Read(const json::value_t& value) override;

protected:
    float m_speed = 100.0f;
};