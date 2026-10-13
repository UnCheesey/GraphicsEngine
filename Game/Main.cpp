#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// Files use namespace fe::

using namespace nu;

namespace nu {
    struct Color4 {
        float r, g, b, a;
    };
}

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{ -0.7f, -0.5f, 0.0f }, Color4{ 1, 0, 0, 1} }, // Bottom-Left
    Vertex{ Vector3{ 0.8f, -0.8f, 0.0f }, Color4{ 0, 1, 0, 1} }, // Bottom-Right
    Vertex{ Vector3{ 0.3f,  0.3f, 0.0f }, Color4{ 0, 0, 1, 1} }, // Top-Middle  
};

struct vertexUniform {
    glm::mat4 model;

};

int main() {
    // SET DIRECTORY
    fe::SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer().GetGPUDevice());

    auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));

    // position
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, position));

    // color
    pipeline->AddVertexAttribute(
        1,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
        offsetof(Vertex, color));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    vertexUniform uniform;
    Color4 color{0, 0, 0, 1};

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
        float time = Engine::Get().GetTime().GetTime();

        // GLM TRANSFORMATION
        // uniform.model = glm::mat4(1.0f);

        auto mxt = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        /*auto mxr = glm::rotate(glm::mat4(1.0f), time * 100.0f, glm::vec3(1.0f, 1.0f, 1.0f));*/
        auto mxr = glm::eulerAngleXYZ(1.2f, time * 3.0f, 0.0f);
        auto mxs = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

        uniform.model = mxt * mxr * mxs;

       /* uniform.model = glm::translate(uniform.model, glm::vec3(0.0f, 0.0f, 0.0f));
        uniform.model = glm::rotate(uniform.model, time * 3.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));*/

        color.r = (float)(sin(time * 10.0) * 0.5 + 0.5);

        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexBuffer(*vb);
        Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));

        Engine::Get().GetRenderer().Draw(vb->GetVertexCount());

        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();    

    return 0;
}