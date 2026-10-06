#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include "Resources/ResourceManager.h"

class Player;

class SpaceGame : public nu::Game {

public:
	enum class GameState {
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	SpaceGame() = default;
	SpaceGame(nu::Scene* scene) : Game(scene) {};

	bool Initialize() override;

	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;
	void AddPoints(int points) { m_score += points; }

private:

	void SpawnPickup();
	void OnPlayerDead();
	void SpawnPlayer();
	void SpawnEnemy();

private:
	Player* m_player;
	int m_score = 0;
	int m_lives = 0;

	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;
	float m_pickUpTimer = 8.0f;
	int m_spawnCount = 5;

	GameState m_gameState = GameState::Title;


	nu::Text* m_titleText{ nullptr };

	nu::Text* m_gameOverText{ nullptr };
		
	nu::Text* m_scoreText{ nullptr };
	nu::Text* m_livesText{ nullptr };
	nu::Text* m_weaponText{ nullptr };
	nu::Text* m_playText{ nullptr };

};