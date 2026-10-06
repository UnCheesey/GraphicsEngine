#include "pch.h"
#include "TilemapRendererComponent.h"
#include "Renderer/Tilemap.h"
#include "Physics/PhysicsBody.h"
#include "Engine.h"

namespace nu {

	FACTORY_REGISTER(TilemapRendererComponent)

	TilemapRendererComponent::TilemapRendererComponent(const TilemapRendererComponent& other) {
		m_tilemapName = other.m_tilemapName;
	}

	void TilemapRendererComponent::Start() {
		m_tilemap = Resources().Get<Tilemap>(m_tilemapName, Engine::Get().GetRenderer());

		if (!m_tilemap) {
			std::cerr << "Could not load tilename" << m_tilemapName << std::endl;
			return;
		}

		Transform transform = GetOwner()->GetTransform();

		PhysicsBody::PhysicsBodyDef bodyDef;
		bodyDef.isDynamic = false;
		bodyDef.actor = GetOwner();

		for (auto layer : m_tilemap->GetLayers()) {
			if (!layer.hasCollision) continue;

			for (int i = 0; i < layer.data.size(); i++) {
				int tileId = layer.data[i];
				if (tileId == 0) continue;

				Rect sourceRect = m_tilemap->GetTileRect(layer, tileId);

				transform.position = GetOwner()->GetTransform().position + (m_tilemap->GetTilePosition(layer, i) * transform.scale);

				Vector2 size{ sourceRect.w, sourceRect.h };

				auto physicsBody = std::make_unique<PhysicsBody>(transform, size, bodyDef, Engine::Get().GetPhysics());

				m_physicsBodies.push_back(std::move(physicsBody));
			}
		}
	}

	void TilemapRendererComponent::Draw(const Renderer& renderer) {
		for (auto layer : m_tilemap->GetLayers()) {
			for (int i = 0; i < layer.data.size(); i++) {
				int tileId = layer.data[i];

				if (tileId == 0) continue;

				Rect sourceRect = m_tilemap->GetTileRect(layer, tileId);

				Transform transform = GetOwner()->GetTransform();
				Vector2 position = transform.position + m_tilemap->GetTilePosition(layer, i) * transform.scale;

				renderer.DrawTexture(*layer.texture, sourceRect, position.x, position.y, transform.rotation, transform.scale);
			}
		}
	}

	void TilemapRendererComponent::Read(const json::value_t& value) {
		RendererComponent::Read(value);

		JSON_READ_NAME(value, "tilemap_name", m_tilemapName);
	}
}