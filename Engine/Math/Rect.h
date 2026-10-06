#pragma once

namespace nu {
	struct Rect {
		float x, y, w, h;

		Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

		Rect(float x, float y, float w, float h) :
			x(x),
			y(y),
			w(w),
			h(h)
		{}
	};
}