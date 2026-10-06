#pragma once
#include "Framework/Actor.h"

enum class WeaponState {
    Default,
    Multishot,
    Rapid,
    Rocket,
};

class Player : public nu::Actor{
public:
    Player() = default;

    CLASS_PROTOTYPE(Player)

    const float GetSpeed() const { return m_speed; }

    void SetWeaponState(WeaponState weaponState) { m_weaponState = weaponState; }
    WeaponState GetWeaponState() const { return m_weaponState; }
       
    void Update(float dt) override;
    void SpawnBullet(float offset = 0.0f);
    void SpawnRocket();

    void Fire();
    void OnCollision(Actor* other) override;

    void Read(const json::value_t& value) override;

    std::string WeaponToString();

private:
    WeaponState m_weaponState = WeaponState::Default;
    float m_fireTimer = 0.0f;
	int m_ammo = 0;
    float m_speed = 800.0f;
};