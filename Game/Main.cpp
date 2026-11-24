#include "Engine.h"
#include "Core/File.h"

using namespace nu;

int main() {
    // SET DIRECTORY -- Files use namespace fe::
    fe::SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();

    Scene scene;
    scene.Load("scenes/scene-skybox.json");

    auto& renderer = Engine::Get().GetRenderer();
   
    // MAIN LOOP
    bool quit = false;
    while (!quit) {

        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            Engine::Get().GetEditor().ProcessEvent(event);
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }        
                        
        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        // SCENE
        scene.Update(dt);

        // EDITOR
        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update(scene);

        // RENDER
        renderer.BeginFrame();

        scene.Draw(renderer);

        Engine::Get().GetRenderer().EndRenderPass();
        Engine::Get().GetEditor().Draw(renderer);

        renderer.EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();    

    return 0;
}