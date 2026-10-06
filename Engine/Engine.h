#pragma once

// Factory
#include "Core/Factory.h"

// Utilities
#include "Math/Random.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Core/File.h"
#include "Core/StringUtils.h"
#include "Serialization/Json.h"

// Systems
#include "Core/GameTime.h"
#include "Renderer/Renderer.h"
#include "Renderer/Font.h"
#include "Renderer/ParticleSystem.h"
#include "Physics/Physics.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "Renderer/Texture.h"
#include "Renderer/Text.h"

// Game
#include "Framework/Game.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"

namespace nu {
	class Engine {
	public:
		static Engine& Get() { static Engine engine; return engine; }

		bool Initialize();
		void Shutdown();

		void Update();

		Renderer& GetRenderer() { return m_renderer; }
		Input& GetInput() { return m_input; }
		Audio& GetAudio() { return m_audio; }
		Time& GetTime() { return m_time; }
		ParticleSystem& GetParticleSystem() { return m_particleSystem;  }
		Physics& GetPhysics() { return m_physics; }

		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;


	private:
		Engine() = default;

		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		Time m_time;
		ParticleSystem m_particleSystem;
		Physics m_physics;
	};
}
