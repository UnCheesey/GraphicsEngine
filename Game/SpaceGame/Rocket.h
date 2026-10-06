#pragma once
#include "Bullet.h"

class Rocket : public Bullet {
public:
	Rocket() = default;
	~Rocket() = default;

	CLASS_PROTOTYPE(Rocket)

	void Read(const json::value_t& value) override;

private:
	void Destroy() override;

	void Explode();

	int m_pelletAmount;
};
