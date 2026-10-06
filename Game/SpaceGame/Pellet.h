#pragma once
#include "Bullet.h"
#include "Core/Factory.h"

class Pellet : public Bullet {
public:
	Pellet() = default;

	CLASS_PROTOTYPE(Pellet)

	void Update(float dt) override;

	void Read(const json::value_t& value) override;
};