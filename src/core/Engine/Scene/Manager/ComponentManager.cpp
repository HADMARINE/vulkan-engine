//
// Created by SongEunwoo on 2020/10/12.
//

#include "../../../../include/Engine/Scene/Manager/ComponentManager.h"
#include "../../../../include/Engine/CoreDefine.h"
#include "../../../../include/Engine/Scene/Components/Base/Component.h"
using namespace SharkEngine::Core;

template<typename T>
void ComponentManager:: RegisterComponent() {
    const char* typeName = typeid(T).name();

    m_ComponentIDs.insert(pair<const char*, ComponentID>(typeName, m_nextComponentID));
    m_ComponentArrays.insert(pair<const char*, shared_ptr<IComponentArray>>(typeName, make_shared<ComponentArray<T>>()));
    ++m_nextComponentID;
}

template<typename T>
ComponentID ComponentManager::GetComponentID() {
    const char* typeName = typeid(T).name();
    return m_ComponentIDs[typeName];
}

template<typename T>
void ComponentManager::AddComponent(EntityID _id, T* component) {
    GetComponentArray<T>()->AddComponent(_id, component);
}
template<typename T>
T* ComponentManager::GetComponent(EntityID _id) {
    return GetComponentArray<T>()->GetComponent(_id);
}

template<typename T>
void ComponentManager::DestroyComponent(EntityID _id) {
    GetComponentArray<T>()->DestroyComponent(_id);
}

void ComponentManager::EntityDestroyed(EntityID _id) {
    for (auto const& pair : m_ComponentArrays)
    {
        auto const& component = pair.second;

        component->EntityDestroyed(_id);
    }
}
template<typename T>
shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray() {
    return shared_ptr<ComponentArray<T>>();
}
void ComponentManager::Update() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            compoIter->Update();
        }
    }
}
void ComponentManager::LateUpdate() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            compoIter->LateUpdate();
        }
    }
}
void ComponentManager::Render() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            compoIter->Render();
        }
    }
}
void ComponentManager::EndScene() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for (auto compoIter : *componentArray) {
            compoIter->EndScene();
        }
    }
}
ComponentManager::~ComponentManager() {
    for(auto const& pair : m_ComponentArrays) {
        auto const& components = pair.second;

        auto const& componentArray = components->GetComponentArray();

        for(auto compoIter : *componentArray) {
            SAFE_DELETE(compoIter);
        }
    }
}
