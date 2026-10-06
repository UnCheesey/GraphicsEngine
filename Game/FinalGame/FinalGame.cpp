#include "FinalGame.h"
#include "Engine.h"
#include <iostream>
#include <memory>

using namespace nu;

bool FinalGame::Initialize() {
    fe::SetWorkingDirectory("FinalGame");

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

void FinalGame::Update(float dt) {      
    switch (m_gameState)
    {
    case GameState::Title:
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
            m_gameState = GameState::StartGame;
        }
        break;
    case GameState::StartGame:
        m_gameState = GameState::StartLevel;
        break;
    case GameState::StartLevel:
        m_scene->RemoveAllActors();
        m_gameState = GameState::Game;
        break;
    case GameState::Game:

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

void FinalGame::Draw(Renderer& renderer) {

    renderer.DrawTexture(*Resources().Get<Texture>("Textures/background.png", Engine::Get().GetRenderer()), renderer.GetWidth() / 2.0f, renderer.GetHeight() / 2.0f);

    switch (m_gameState) {

    case FinalGame::GameState::Title:
        break;
    case FinalGame::GameState::StartGame:
        break;
    case FinalGame::GameState::StartLevel:
        break;
    case FinalGame::GameState::Game:
        break;
    case FinalGame::GameState::GameOver:
        break;
    default:
        break;
    }

    Engine::Get().GetParticleSystem().Draw(Engine::Get().GetRenderer());
    m_scene->Draw(renderer);    
}

//void FinalGame::OnPlayerDead() {
//    m_lives--;
//    m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;
//}
//
//void FinalGame::SpawnPlayer() {
//    auto actor = Factory::Instance().Create<Player>("PlayerPrototype");
//
//    if (actor) {
//        m_player = actor.get();
//        m_scene->AddActor(std::move(actor));
//        
//    }
//}
//
//void finalGame::SpawnEnemy() {
//    auto actor = Factory::Instance().Create<Enemy>("EnemyPrototype");
//    if (actor) {
//    actor->SetPosition({ nu::RandomFloat(Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat(Engine::Get().GetRenderer().GetHeight()) });
//    m_scene->AddActor(std::move(actor));    
//    }
//}
