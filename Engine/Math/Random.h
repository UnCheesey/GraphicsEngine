#pragma once

#include <cstdlib>
#include <random>

namespace nu {

	inline std::mt19937& Generator() {
		static std::random_device randomDevice;
		static std::mt19937 generator(randomDevice());

		return generator;
	}

	inline void SeedRandom(unsigned int seed) {
		Generator().seed(seed);
	}

	inline int RandomInt() {
		static std::uniform_int_distribution<> dist;
		return dist(Generator());
	}

	inline int RandomInt(int max) {
		std::uniform_int_distribution<> dist(0, max - 1);
		return dist(Generator());
	}

	inline int RandomInt(int min, int max) {
		if (min > max) std::swap(min, max);

		std::uniform_int_distribution<> dist(min, max);
		return dist(Generator());
	}

	inline float RandomFloat() {
		static std::uniform_real_distribution<> dist(0.0f, 1.0f);
		return static_cast<float>(dist(Generator()));
	}

	inline float RandomFloat(float max) {
		std::uniform_real_distribution<> dist(0.0f, max);
		return static_cast<float>(dist(Generator()));
	}

	inline float RandomFloat(float min, float max) {
		if (min > max) std::swap(min, max);

		std::uniform_real_distribution<> dist(min, max);
		return static_cast<float>(dist(Generator()));
	}

	inline bool RandomBool() {
		std::uniform_real_distribution<> dist(0.05);
		return dist(Generator());
	}

	inline float RandomColor() {
		return RandomFloat(1.0f);
	}
}