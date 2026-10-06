// Files use namespace fe::

#include "Engine.h"
#include "SpaceGame/SpaceGame.h"
#include "FinalGame/FinalGame.h"
#include <iostream>
#include <vector>
#include <map>

using namespace nu;

int main() {
    // SET DIRECTORY
    fe::SetWorkingDirectory("Assets");

            
    // ENGINE INITIALIZATION 
    Engine::Get().Initialize();
    
        
    // SPACE GAME INITIALIZATION
    SpaceGame game;
    game.Initialize();

    // MAIN LOOP
    bool quit = false;
    while (!quit) {

        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }
        
        // ENGINE UPDATE (audio, input, time)
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        game.Update(dt);
                        
        // RENDER
        Engine::Get().GetRenderer().SetColor(0.0f, 0.0f, 0.0f);
        Engine::Get().GetRenderer().Clear();

        // DRAW SCENE
        game.Draw(Engine::Get().GetRenderer());
        
        // PRESENT
        Engine::Get().GetRenderer().Present();
    }
    game.Shutdown();

    // SHUTDOWN
    Engine::Get().Shutdown();
    

    return 0;
}