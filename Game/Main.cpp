#include "Engine.h"
#include "Core/File.h"
// Files use namespace fe::

using namespace nu;

int main() {
    // SET DIRECTORY
    fe::SetWorkingDirectory("Assets");

            
    // ENGINE INITIALIZATION 
    Engine::Get().Initialize();

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
                        
        // RENDER
        Engine::Get().GetRenderer().BeginFrame();
        
        // PRESENT
        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();
    

    return 0;
}