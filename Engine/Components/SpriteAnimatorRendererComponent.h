#pragma once
#include "SpriteRendererComponent.h"
#include "Resources/ResourceManager.h"

namespace nu {
	class SpriteAnimatorRendererComponent : public SpriteRendererComponent {
	public:
		CLASS_PROTOTYPE(SpriteRendererComponent)

	public:
		struct SpriteAnimation {
			std::string name;
			float fps = 10.0f;
			bool loop = true;
			res_t<class TextureFrame> textureFrames;
		};

	public:

		void Start() override;
		void Update(float dt) override;

		void Play(const std::string name);

		void SetPause(bool pause = true) { m_pause = pause; }
		bool IsPaused() const { return m_pause; }

		void Read(const json::value_t& value) override;


	private:
		unsigned int m_frame = 0;
		float m_frameTimer = 0;
		bool m_pause = false;

		std::string m_defaultAnimationName;

		SpriteAnimation m_spriteAnimation;
		std::map<std::string, SpriteAnimation> m_spriteAnimations;
	};
}