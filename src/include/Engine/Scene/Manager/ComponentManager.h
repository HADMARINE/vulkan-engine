//
// Created by SongEunwoo on 2020/10/12.
//

//#ifndef VULKAN_ENGINE_COMPONENTMANAGER_H
//#define VULKAN_ENGINE_COMPONENTMANAGER_H

#pragma once

#include "../../ComponentArray.hpp"
#include <shared_mutex>
#include <unordered_map>
#include <glm/glm.hpp>


namespace SharkEngine::Core {
    class ComponentManager {
    public:
        ~ComponentManager();

        template<typename T>
        void RegisterComponent(){
            const char* typeName = typeid(T).name();

            m_ComponentIDs.insert(std::pair<const char*, ComponentID>(typeName, m_nextComponentID));
            m_ComponentArrays.insert(pair<const char*, std::shared_ptr<IComponentArray>>(typeName, std::make_shared<ComponentArray<T>>()));
            ++m_nextComponentID;
        }

        template<typename T>
        ComponentID GetComponentID(){
            const char* typeName = typeid(T).name();
            return m_ComponentIDs[typeName];
        }

        template<typename T>
        void AddComponent(EntityID _id, Component* component) {
            GetComponentArray<T>()->AddComponent(_id, component);
        }

        template<typename T>
        T* GetComponent(EntityID _id) {
            return GetComponentArray<T>()->GetComponent(_id);
        }

        template<typename T>
        void DestroyComponent(EntityID _id){
            GetComponentArray<T>()->DestroyComponent(_id);
        }

        void EntityDestroyed(EntityID _id){
            for (auto const& pair : m_ComponentArrays)
            {
                auto const& component = pair.second;

                component->EntityDestroyed(_id);
            }
        }

        void Start();
        void Update();
        void LateUpdate();
        void Render();
        void EndScene();

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            assert(m_ComponentArrays.find(typeName) != m_ComponentArrays.end() && "Component not registered before use.");

            return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
        }

    private:
        std::unordered_map<const char*, ComponentID>m_ComponentIDs{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays{};

        ComponentID m_nextComponentID;
    };
}

//#endif//VULKAN_ENGINE_COMPONENTMANAGER_H
