#pragma once
#include "Framework/Actor.h"
#include "Player.h"
#include "Math/Random.h"

class Pickup : public nu::Actor {
public:
	Pickup() {
	};

	Pickup(const Pickup& other) : Actor(other){
		m_weaponType = static_cast<WeaponState>(nu::RandomInt(4));
	}

	WeaponState GetWeapon() const { return m_weaponType; }

	CLASS_PROTOTYPE(Pickup)
private:
	WeaponState m_weaponType = WeaponState::Default;
};
