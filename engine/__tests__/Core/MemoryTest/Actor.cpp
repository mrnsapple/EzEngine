/**
 * @file "GameObjects/Private/Actor.cpp"
 *
 * @author Oriol Manzano
 *
 * CHANGELOG :
 *
 * 16/10/2020
 * DELETING default constructor and destructor (Alexandre Chetafi)
 * Relocating static definition from hpp to cpp (here) (Alexandre Chetafi)
 *
**/

#include "Actor.hpp"

namespace ez
{
	void * Actor::operator new(size_t size)
	{
		PoolManager *s = PoolManager::getInstance();

		//free(size);
		//return malloc(size);
		return s->Allocate(size);
	}

	void Actor::operator delete(void *ptr, size_t size)
	{
		PoolManager *s = PoolManager::getInstance();
		//free(ptr);
		return s->Dellocate(ptr);
	}
}