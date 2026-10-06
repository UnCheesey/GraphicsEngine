#pragma once

namespace nu {

	constexpr float Pi = 3.1415926535897932384626433832795f;
	constexpr float TwoPi = Pi * 2;
	constexpr float HalfPi = Pi / 2;

	constexpr float RadToDeg = 180.0f / Pi;
	constexpr float DegToRad = Pi / 180.0f;

	template<typename T>
	inline T Min(T a, T b) {
		return (a < b) ? a : b;
	}

	template<typename T>
	inline T Max(T a, T b) {
		return (a > b) ? a : b;
	}

	template<typename T>
	inline T Wrap(T min, T max, T value) {
		if (value > max) value = min;
		if (value < min) value = max;

		return value;
	}

	template<typename T>
	inline T Clamp(T min, T max, T value) {
		if (value > max) value = max;
		if (value < min) value = min;

		return value;
	}
}
