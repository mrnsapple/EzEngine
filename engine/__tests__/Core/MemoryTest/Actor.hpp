/**
 * @file "GameObjects/Public/Actor.hpp"
 *
 * @author Oriol Manzano
 *
 * CHANGELOG :
 *
 * 16/10/2020
 * Making constructor and destructor by default, using default keyword, not default implementation (Alexandre Chetafi)
 * Relocating new and delete Definition to Actor.cpp (Alexandre Chetafi)
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "PoolManager.hpp"

namespace ez
{
	class Actor
	{
	public:
	  // Object data, 16 bytes:
	  Actor(unsigned long a) {}
	  Actor() {};
	  virtual ~Actor() {};
	  uint64_t data[2];

	  // Declare out custom allocator for
	  // the `Object` structure:
		static void *operator new(size_t size);
		static void operator delete(void *ptr, size_t size);
	};
}

#endif
