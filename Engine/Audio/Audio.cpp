#include "pch.h"
#include "Audio.h"
#include "fmod_errors.h"

namespace nu {

	bool Audio::Initialize() {
		FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
		if (!CheckFMODResult(result)) return false;

		void* extradriverdata = nullptr;
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
		if (!CheckFMODResult(result)) return false;

		return true;
	}

	void Audio::Shutdown() {
		CheckFMODResult(m_fmodSystem->release());
	}

	void Audio::Update() {
		CheckFMODResult(m_fmodSystem->update());
	}

	bool Audio::AddSound(const std::string& name, const std::string& filename, bool loop) {
		if (m_sounds.contains(name)) {
			std::cerr << "Audio System: Name \'" << name << "\' already exists" << std::endl;
			return false;
		}

		// Add extension
		/*
		
		
		
		*/

		FMOD_MODE fmodState = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;

		FMOD::Sound* sound = nullptr;
		FMOD_RESULT result = m_fmodSystem->createSound(filename.c_str(), fmodState, 0, &sound);
		if (!CheckFMODResult(result)) return false;

		m_sounds[name] = sound;

		return true;
	}

	bool Audio::PlaySound(const std::string& name) {
		if (!m_sounds.contains(name)) {
			std::cerr << "Audio System: Name \'" << name << "\' does not exist" << std::endl;
			return false;
		}

		FMOD_RESULT result = m_fmodSystem->playSound(m_sounds[name], nullptr, false, nullptr);
		if (!CheckFMODResult(result)) return false;

		return true;
	}

	bool Audio::CheckFMODResult(FMOD_RESULT result) {
		if (result != FMOD_OK) {
			std::cerr << FMOD_ErrorString(result) << std::endl;
			return false;
		}

		return true;
	}
}