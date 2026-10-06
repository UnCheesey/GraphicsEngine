#pragma once
#include "Font.h"
#include "Math/Vector3.h"

	struct SDL_Texture;

namespace nu{

	class Renderer;

	class Text {

	public:
		Text() = default;
		Text(res_t<Font> font) : m_font{ font } {}
		~Text();

		bool Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(const Renderer& renderer, float x, float y);

		float GetTextWidth();
		float GetTextHeight();

	private:
		res_t<Font> m_font{ nullptr };
		SDL_Texture* m_texture{ nullptr };
	};
}
