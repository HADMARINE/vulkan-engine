//
// Created by SongEunwoo on 2020/10/12.
//

#ifndef VULKAN_ENGINE_COMPONENTARRAY_HPP
#define VULKAN_ENGINE_COMPONENTARRAY_HPP

#include "../../CoreDefine.h"
#include "../../CoreTypes.h"
#include "../Components/Base/Component.h"
#include <unordered_map>
#include <array>
#include <cassert>

namespace SharkEngine::Core {
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(EntityID _id) = 0;
        virtual std::array<Component *, MAX_COMPONENTS> *GetComponentArray() = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void AddComponent(EntityID _id, Component *component) {
            assert(m_EntityToIndexMap.find(_id) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

            // Put new entry at end and update the maps
            size_t newIndex = m_Size;
            m_EntityToIndexMap[_id] = newIndex;
            m_IndexToEntityMap[newIndex] = _id;
            m_ComponentArray[newIndex] = component;

            m_Size++;
        }

        void DestroyComponent(EntityID _id) {
            assert(m_EntityToIndexMap.find(_id) == m_EntityToIndexMap.end() && "Removing non-existent component");

            size_t indexOfRemovedEntity = m_EntityToIndexMap[_id];
            size_t indexOfLastElement = m_Size - 1;
            SAFE_DELETE(
                    m_ComponentArray[indexOfRemovedEntity]);
            m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

            EntityID entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
            m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            m_EntityToIndexMap.erase(_id);
            m_IndexToEntityMap.erase(indexOfLastElement);

            --m_Size;
        }

        T *GetComponent(EntityID _id = -1) {
            assert(m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end() && "Retrieving non-existent component");

            return m_ComponentArray[m_EntityToIndexMap[_id]];
        }

        void EntityDestroyed(EntityID _id) override {
            if (m_EntityToIndexMap.find(_id) != m_EntityToIndexMap.end()) {
                // Remove the entity's component if it existed
                DestroyComponent(_id);
            }
        }

        std::array<Component *, MAX_COMPONENTS> *GetComponentArray() {
            return &m_ComponentArray;
        }

    private:
        std::array<Component *, MAX_COMPONENTS> m_ComponentArray;
        std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
        std::unordered_map<size_t, EntityID> m_IndexToEntityMap;

        // Total size of valid entries in the array.
        size_t m_Size;
    };
}// namespace SharkEngine::Core

#endif//VULKAN_ENGINE_COMPONENTARRAY_HPP