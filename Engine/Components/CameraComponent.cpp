#include "pch.h"
#include "CameraComponent.h"
#include "Core/Factory.h"
#include "Framework/Actor.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(CameraComponent)

	void CameraComponent::Update(float dt)
	{
		m_view = glm::lookAtRH(m_owner->GetTransform().position, m_owner->GetTransform().position + glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f , 1.0f , 0.0f});
		float aspectRation = Engine::Get().GetRenderer().GetWidth() / static_cast<float>(Engine::Get().GetRenderer().GetHeight());
		m_projection = glm::perspective(glm::radians(m_fov), aspectRation, m_nearPlane, m_farPlane);

	}
	void CameraComponent::Draw(const Renderer& renderer)
	{

	}

	void CameraComponent::Read(const json::value_t& value)
	{
		RendererComponent::Read(value);

		JSON_READ_NAME(value, "fov", m_fov);
		JSON_READ_NAME(value, "near_plane", m_nearPlane);
		JSON_READ_NAME(value, "far_plane", m_farPlane);
	}
}