#pragma once
#include "Resource.h"
#include "Core/Singleton.h"

#include <string>
#include <map>
#include <iostream>

namespace nu {
	class ResourceManager : public Singleton<ResourceManager>{
	public:
		void RemoveAll() { m_resources.clear(); }

		template<typename T, typename ... Args>
			requires std::derived_from<T, Resource>
		res_t<T> Get(const std::string& name, Args&& ... args);

		template<typename T, typename ... Args>
			requires std::derived_from<T, Resource>
		res_t<T> GetWithID(const std::string& id, const std::string& name, Args&& ... args);

		const auto& GetAll() const { return m_resources; }

		void Clear() { m_resources.clear(); }

	private:
		std::map<std::string, res_t<Resource>> m_resources;
	};

	template<typename T, typename ... Args>
		requires std::derived_from<T, Resource>
	inline res_t<T> ResourceManager::Get(const std::string& name, Args&& ... args) {
		return GetWithID<T>(name, name, std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
		requires std::derived_from<T, Resource>
	inline res_t<T> ResourceManager::GetWithID(const std::string& id, const std::string& name, Args && ...args) {
		auto iter = m_resources.find(id);

		// check if exists
		if (iter != m_resources.end()) {
			auto base = iter->second;
			auto resource = std::dynamic_pointer_cast<T>(base);
			if (resource == nullptr) {
				std::cerr << "Resource type mismatch: " << id << std::endl;
				return res_t<T>();
			}

			resource->SetName(name);
			m_resources[name] = resource;

			// return resource
			return resource;
		}

		// load resource
		res_t<T> resource = std::make_shared<T>();
		if (resource->Load(name, std::forward<Args>(args)...) == false)
		{
			std::cerr << "Could not load resource: " << name << std::endl;
			return res_t<T>();
		}

		// add resource to resource manager
		resource->SetName(id);
		m_resources[id] = resource;

		return resource;
	}

	inline ResourceManager& Resources() { return ResourceManager::Instance(); }

}