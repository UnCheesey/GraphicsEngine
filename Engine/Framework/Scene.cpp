#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "Engine.h"
#include "Core/Factory.h"
#include "Renderer/Renderer.h"
#include "Renderer/Light.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"


namespace nu {
	void Scene::Update(float dt) {
		// UPDATE ACTORS
		for (auto& actor : m_actors) {
			actor->Update(dt);
		}

		// REMOVE DESTROYED ACTORS
		for (auto& actor : m_actors) {
			if (actor->IsDestroyed()) actor->Destroy();
		}

		std::erase_if(m_actors, [](auto& actor) { return actor->IsDestroyed(); });

		// ADD PENDING ACTORS
		for (auto& actor : m_pendingActors) {
			actor->Start();
			m_actors.push_back(std::move(actor));
		}
		m_pendingActors.clear();
	}

	void Scene::Draw(const Renderer& renderer) {
		FrameUniform frameUniform{};

		auto camera = GetActorByName<Actor>("Camera");
		if (camera) {
			auto cameraComponent = camera->GetComponent<CameraComponent>();
			if (cameraComponent) {
				frameUniform.cameraPosition = camera->GetTransform().position;
				frameUniform.view = cameraComponent->GetView();
				frameUniform.projection = cameraComponent->GetProjection();
				frameUniform.viewProjection = frameUniform.projection * frameUniform.view;

				renderer.SetVertexUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
				renderer.SetFragmentUniform(UniformSlot::Frame, &frameUniform, sizeof(frameUniform));
			}
		}

		LightUniform lightUniform{};
		lightUniform.ambient = m_ambientLight;

		auto lights = GetActorsByTag("Light");

		for (auto light : lights)
		{
			if (!light->IsActive()) continue;

			auto lightComponent = light->GetComponent<LightComponent>();
			if (lightComponent && lightComponent->IsActive())
			{
				lightUniform.lights[lightUniform.count++] = lightComponent->GetLightData();
			}
		}
		renderer.SetVertexUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));
		renderer.SetFragmentUniform(UniformSlot::Light, &lightUniform, sizeof(lightUniform));

		for (auto& actor : m_actors) {
			if (actor->IsActive()) actor->Draw(renderer);
		}
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor){
		actor->m_scene = this;
		m_pendingActors.push_back(std::move(actor));
	}

	void Scene::RemoveAllActors(bool force) {
		std::erase_if(m_actors, [force](auto& actor) { return !actor->IsPersistent() || force; });
	}

	bool Scene::Load(const std::string& sceneName) {
		json::document_t document;
		if (json::Load(sceneName, document)) {
			if (JSON_HAS_NAME(document, "actors")) {
				for (auto& actorValue : document["actors"].GetArray()) {

					// get actor type
					std::string typeName;
					JSON_READ_NAME(actorValue, "type", typeName);

					std::cout << "Loading actor type: " << typeName << std::endl;

					// create actor of type
					auto actor = Factory::Instance().Create<Actor>(typeName);

					if (!actor) {
						std::cout << "Could not create actor type: " << typeName << std::endl;
						continue;
					}

					// read actor json
					actor->Read(actorValue);

					// check if prototype
					bool prototype = false;
					JSON_READ(actorValue, prototype);

					if (prototype) {
						// if prototype, add prototype to factory registry
						std::string name;
						JSON_READ(actorValue, name);
						Factory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else {
						AddActor(std::move(actor));
					}
				}
			}
			return true;
		}
		else {
			return false;
		}
	}

	void Scene::UpdateGui()
	{
		glm::vec3 clearColor = Engine::Get().GetRenderer().GetClearColor();

		ImGui::ColorEdit3("Clear Color", glm::value_ptr(clearColor));
		Engine::Get().GetRenderer().SetClearColor(clearColor);


		ImGui::ColorEdit3("Ambient Light", glm::value_ptr(m_ambientLight));
	}
}