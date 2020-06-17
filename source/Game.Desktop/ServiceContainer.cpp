#include "pch.h"
#include "ServiceContainer.h"

namespace Library
{
	void ServiceContainer::AddService(uint64_t typeId, void* service)
	{
		mServices.insert(std::pair<uint64_t, void*>(typeId, service));
	}

	void ServiceContainer::RemoveService(uint64_t typeId)
	{
		mServices.erase(typeId);
	}

	void* ServiceContainer::GetService(uint64_t typeId) const
	{
		std::map<uint64_t, void*>::const_iterator it = mServices.find(typeId);

		return (it != mServices.end() ? it->second : nullptr);
	}
}
