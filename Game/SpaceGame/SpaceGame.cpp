#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include <iostream>
#include <memory>

using namespace nu;

bool SpaceGame::Initialize() {
    fe::SetWorkingDirectory("SpaceGame");

    Game::Initialize();
    
    m_scene = std::make_unique<Scene>();
    m_scene->SetGame(this);
    m_scene->Load("Scenes/scene.json");
    

    m_titleText = new Text(Resources().GetWithID<Font>("title_font", "Fonts/Blaster.ttf", 64.0f));
    m_scoreText = new Text(Resources().GetWithID<Font>("game_font", "Fonts/Blaster.ttf", 32.0f));
    m_livesText = new Text(Resources().GetWithID<Font>("game_font", "Fonts/Blaster.ttf", 32.0f));
    m_weaponText = new Text(Resources().GetWithID<Font>("game_font", "Fonts/Blaster.ttf", 32.0f));
    m_gameOverText = new Text(Resources().GetWithID<Font>("gameover_font", "Fonts/Blaster.ttf", 64.0f));
    m_playText = new Text(Resources().GetWithID<Font>("play_font", "Fonts/Blaster.ttf", 32.0f));

    m_playText->Create(Engine::Get().GetRenderer(), "Press SPACE to play", Color{ 1.0f, 0.5f, 0.01f });

    Engine::Get().GetAudio().AddSound("background", "Sounds/space_background.mp3", true);
    Engine::Get().GetAudio().AddSound("laser", "Sounds/laser.wav");
    Engine::Get().GetAudio().PlaySound("background");

    return true;
}

void SpaceGame::Update(float dt) {      
    switch (m_gameState)
    {
    case GameState::Title:
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
            m_gameState = GameState::StartGame;
        }
        break;
    case GameState::StartGame:
        m_score = 0;
        m_lives = 1;
        m_spawnTime = 5.0f;
        m_pickUpTimer = 3.0f;
        m_gameState = GameState::StartLevel;
        break;
    case GameState::StartLevel:
        m_scene->RemoveAllActors();
        SpawnPlayer();
        m_gameState = GameState::Game;
        break;
    case GameState::Game:
        m_spawnTimer -= dt;
        m_pickUpTimer -= dt;
        if (m_spawnTimer <= 0.0f) {
            m_spawnTimer = m_spawnTime;
            SpawnEnemy();
        }

        if (m_pickUpTimer <= 0.0f) {
            SpawnPickup();
            m_pickUpTimer = RandomFloat(8.0f, 15.0f);
        }

        if (m_scene->GetActorByTag<Player>("Player") == nullptr) {
            OnPlayerDead();
        }

        break;
    case GameState::GameOver:
        m_scene->RemoveAllActors();
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
            m_gameState = GameState::StartGame;
        }

        break;
    default:
        break;
    }

    Game::Update(dt);
}

void SpaceGame::Draw(Renderer& renderer) {

    renderer.DrawTexture(*Resources().Get<Texture>("Textures/background.png", Engine::Get().GetRenderer()), renderer.GetWidth() / 2.0f, renderer.GetHeight() / 2.0f);

    switch (m_gameState) {

    case SpaceGame::GameState::Title:
        m_titleText->Create(Engine::Get().GetRenderer(), "Game Engine", Color{ 1.0f, 0.0f, 0.0f });
        m_titleText->Draw(renderer, (renderer.GetWidth() - m_titleText->GetTextWidth()) / 2.0f, (renderer.GetHeight() - m_titleText->GetTextHeight()) / 2.0f);
        m_playText->Draw(renderer, (renderer.GetWidth() - m_playText->GetTextWidth()) / 2.0f, (renderer.GetHeight() / 2.0f) + m_titleText->GetTextHeight());
        break;
    case SpaceGame::GameState::StartGame:
        break;
    case SpaceGame::GameState::StartLevel:
        break;
    case SpaceGame::GameState::Game:
        m_scoreText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
        m_scoreText->Draw(renderer, 30, 30);

        m_livesText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
        m_livesText->Draw(renderer, renderer.GetWidth() - m_livesText->GetTextWidth() - 30.0f, 30.0f);
        if (m_player) {
            m_weaponText->Create(renderer, "Weapon: " + m_player->WeaponToString(), { 1.0f, 1.0f, 1.0f });
            m_weaponText->Draw(renderer, 30, m_scoreText->GetTextHeight() + 30);
        }
        
        break;
    case SpaceGame::GameState::GameOver:
        m_gameOverText->Create(Engine::Get().GetRenderer(), "Game Over", Color{ 1.0f, 0.0f, 0.0f });
        m_gameOverText->Draw(renderer, (renderer.GetWidth() - m_gameOverText->GetTextWidth()) / 2.0f, (renderer.GetHeight() - m_gameOverText->GetTextHeight()) / 2.0f);
        m_playText->Draw(renderer, (renderer.GetWidth() - m_playText->GetTextWidth()) / 2.0f, (renderer.GetHeight() / 2.0f) + m_gameOverText->GetTextHeight());
        break;
    default:
        break;
    }

    Engine::Get().GetParticleSystem().Draw(Engine::Get().GetRenderer());
    m_scene->Draw(renderer);    
}

void SpaceGame::OnPlayerDead() {
    m_lives--;
    m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;
}

 void SpaceGame::SpawnPickup() {
        auto actor = Factory::Instance().Create<Pickup>("PickupPrototype");

        if (actor) {
            actor->SetPosition({ nu::RandomFloat(Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat(Engine::Get().GetRenderer().GetHeight()) });
            m_scene->AddActor(std::move(actor));
        }
 }

void SpaceGame::SpawnPlayer() {
    auto actor = Factory::Instance().Create<Player>("PlayerPrototype");

    if (actor) {
        m_player = actor.get();
        m_scene->AddActor(std::move(actor));
        
    }
}

void SpaceGame::SpawnEnemy() {
    auto actor = Factory::Instance().Create<Enemy>("EnemyPrototype");
    if (actor) {
    actor->SetPosition({ nu::RandomFloat(Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat(Engine::Get().GetRenderer().GetHeight()) });
    m_scene->AddActor(std::move(actor));    
    }
}
