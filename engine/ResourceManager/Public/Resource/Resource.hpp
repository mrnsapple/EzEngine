#ifndef Resource_hpp
#define Resource_hpp

#include <string>
#include "UniqueID.hpp"

namespace ez{

	class ResourceData;
	struct Event;

	class Resource
	{
		 //Resource(void  *data, std::size_t size){}
		virtual bool UpdateResourceData(const ResourceData &) = 0;
		virtual bool OnEvent(Event &e) = 0;
	private:
		std::shared_ptr<UniqueID> id;
		std::shared_ptr<UniqueID> resourceRegistryId;
	};

	typedef Resource* ( *CreateResourcelFn)(void *, std::size_t);
}
#endif