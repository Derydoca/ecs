#include "pch.h"
#include "EntityManager.h"
#include <cassert>

ECS::EntityManager::EntityManager(const size_t blockSize, const size_t blockCount) :
	m_allocator(blockSize, blockCount)
{
}

ECS::EntityManager::~EntityManager()
{
}

void ECS::EntityManager::CreateEntity(Entity& entity)
{
	assert(entity.m_id <= 0);
	entity.m_id = m_nextEntityId++;
}
