#pragma once
#include "SpriteRendererComponent.h"
#include "Resources/ResourceManager.h"

namespace nu {
	class SpriteAnimationRendererComponent : public SpriteRendererComponent {
	public:
		CLASS_PROTOTYPE(SpriteAnimationRendererComponent)

		void Start() override;
		void Update(float dt) override;

		void Read(const json::value_t& value) override;

	private:
		float m_fps = 1.0f;
		bool m_loop = true;

		unsigned int m_frame = 0;
		float m_frameTimer = 0.0f;

		std::string m_textureFramesName;
		res_t<class TextureFrame> m_textureFrames;
	};
}