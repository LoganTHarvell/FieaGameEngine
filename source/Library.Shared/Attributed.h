#pragma once


#include "RTTI.h"
#include "TypeManager.h"
#include "Scope.h"


namespace Library
{
	class Attributed : public Scope
	{
	public:
		explicit Attributed(const RTTI::IdType typeId);
		virtual ~Attributed() = 0;
		Attributed(const Attributed& rhs);
		Attributed& operator=(const Attributed& rhs);
		Attributed(Attributed&& rhs) noexcept;
		Attributed& operator=(Attributed&& rhs) noexcept;

	private:
		TypeManager::SignaturesType&& Signatures();

		void Populate(const RTTI::IdType typeId);
		void UpdateExternalStorage();
	};
}