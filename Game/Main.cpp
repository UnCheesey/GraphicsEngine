#include "Engine.h"
#include "Core/File.h"

#include "Math/Transform.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#define CHECK_UNIFORM_BOUNDARY(T) static_assert((sizeof(T) % 16) == 0);
#define MAX_LIGHTS 8

using namespace nu;

enum class LightType : uint32_t
{
    Directional,
    Point,
    Spot
};

int main() {
    // SET DIRECTORY -- Files use namespace fe::
    fe::SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();

    auto model = Resources().Get<Model>("models/redhood.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/redhood.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/phong.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/phong.frag", Engine::Get().GetRenderer(), 3, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    // UNIFORMS
    struct VertexUniform
    {
        glm::mat4 mvp;
        glm::mat4 model;
        glm::vec3 cameraPosition;
        float padding1;
    };
    CHECK_UNIFORM_BOUNDARY(VertexUniform)
    VertexUniform vertexUniform;

    struct Light 
    {
        glm::vec3 position;
        LightType type;
        glm::vec3 direction;
        float range;
        glm::vec3 color;
        float intensity;
        glm::vec3 ambient;
        float innerCutoff;
        float outerCutoff;
        glm::vec3 padding;
    };

    struct LightUniform
    {   
        Light lights[MAX_LIGHTS];
        uint32_t numLights;
        glm::vec3 ambient;
    };

    CHECK_UNIFORM_BOUNDARY(LightUniform)
    LightUniform lightUniform;

    Light light1{
        .position{ 10, 10, 10},
        .type{ LightType::Point },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{ 1, 1, 1 },
        .intensity{ 1 },
        .ambient{ 0.1f },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 1.6f }
    };

    Light light2{
        .position{ -10, 10, 10},
        .type{ LightType::Point },
        .direction{0, -1, 0},
        .range{ 40 },
        .color{ 0, 0, 1 },
        .intensity{ 1 },
        .ambient{ 0.1f },
        .innerCutoff{ 0.3f },
        .outerCutoff{ 1.6f }
    };

    lightUniform.lights[0] = light1;
    lightUniform.lights[1] = light2;
    lightUniform.numLights = 2;
    lightUniform.ambient = glm::vec3{ 0.2f };

    struct MaterialUniform
    {
        glm::vec4 color;
        glm::vec2 tiling;
        glm::vec2 offset;
        float shine;
        glm::vec3 padding;
    };

    CHECK_UNIFORM_BOUNDARY(MaterialUniform)

    MaterialUniform materialUniform
    {
        .color{ 1, 1, 1, 1},
        .tiling{ 1.0f, 1.0f},
        .offset{ 0.0f, 0.0f },
        .shine{ 64.0f }
    };

    vertexUniform.cameraPosition = glm::vec3{ 0, 3, 8 };

    Transform transform;
    transform.scale = glm::vec3{ 4.0f };

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
                        
        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.y = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.y = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.z = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.z = +1;

        vertexUniform.cameraPosition += direction * 4.0f * dt;

        lightUniform.lights[0].position = glm::eulerAngleY(time) * glm::vec4{ 0, 0, 10, 1};
        lightUniform.lights[1].position = glm::eulerAngleX(time * 0.5f) * glm::vec4{ 0, 0, 10, 1};

        /*if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) lightUniform.type = LightType::Directional;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) lightUniform.type = LightType::Point;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_3)) {
            lightUniform.type = LightType::Spot;
            lightUniform.position = glm::vec3{ 0, 10, 0 };
            lightUniform.innerCutoff = glm::cos(glm::radians(10.0f));
            lightUniform.outerCutoff = glm::cos(glm::radians(40.0f));
        }*/

        //materialUniform.offset.x = time * .3f;

        // TRANSFORM
        // transform.rotation.y = time * 90;

        // VIEW MATRIX
        auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0.0f, 0.0f, -1.0f }, glm::vec3{ 0.0f , 1.0f , 0.0f });

        // PROJECTION MATRIX
        float aspectRation = Engine::Get().GetRenderer().GetWidth() / static_cast<float>(Engine::Get().GetRenderer().GetHeight());
        auto projection = glm::perspective(glm::radians(70.0f), aspectRation, 0.01f, 200.0f );
                
        // MVP
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;

        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(2, &materialUniform, sizeof(materialUniform));

        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();    

    return 0;
}