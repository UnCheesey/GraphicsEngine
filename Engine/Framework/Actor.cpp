#include "pch.h"
#include "Actor.h"
#include "Math/MathUtils.h"
#include "Renderer/Texture.h"
#include "Engine.h"
#include "Resources/ResourceManager.h"
#include "Components/RendererComponent.h"

namespace nu {

    FACTORY_REGISTER(Actor)

    Actor::Actor(const Actor& other) :
        Object(other),
        m_tag(other.m_tag),
        m_transform(other.m_transform),
        m_lifespan(other.m_lifespan){

        for (const auto& component : other.m_components) {
            auto componentClone = std::unique_ptr<Component>(dynamic_cast<Component*>(component->Clone().release()));
            AddComponent(std::move(componentClone));
        }
    }

    void Actor::Update(float dt) {
            // LIFESPAN

            if (m_lifespan > 0.0f && !IsDestroyed()) {
                m_lifespan -= dt;
                m_destroyed = (m_lifespan <= 0.0f);
            }

            for (auto& component : m_components) {
                component->Update(dt);
            }
    }

    void Actor::Draw(const Renderer& renderer) const {

        for (auto& component : m_components) {
            auto rendererComponent = dynamic_cast<RendererComponent*>(component.get());
            if (rendererComponent) {
                rendererComponent->Draw(renderer);
            }
        }
    }

    void Actor::Start() {
        for (auto& component : m_components) {
            component->Start();
        }
    }

    void Actor::Destroy() {
        for (auto& component : m_components) {
            component->Destroy();
        }
    }

    float Actor::GetRadius() const {

        return 0.0f;
    }

    void Actor::Read(const json::value_t& value) {
              
        Object::Read(value);             
        
        JSON_READ_NAME_REQ(value, "tag", m_tag);
        JSON_READ_NAME(value, "lifespan", m_lifespan);
        JSON_READ_NAME(value, "persistent", m_persistent);

        if (JSON_HAS_NAME(value, "transform")) {
            m_transform.Read(JSON_GET_NAME(value, "transform"));
        }

        if (JSON_HAS_NAME(value, "components")) {
            for (auto& componentValue : JSON_GET_NAME(value, "components").GetArray()) {

                // get component type
                std::string typeName;
                JSON_READ_NAME_REQ(componentValue, "type", typeName);

                std::cout << "Loading component type: " << typeName << std::endl;

                // create component of type
                auto component = Factory::Instance().Create<Component>(typeName);

                if (component) {
                    component->Read(componentValue);
                    AddComponent(std::move(component));
                }
            }
        }
    }
    void Actor::AddComponent(std::unique_ptr<Component> component) {
        component->SetOwner(this);
        m_components.push_back(std::move(component));
    }
}