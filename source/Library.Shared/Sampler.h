#pragma once

#include "Resource.h"

namespace Library
{
	class Sampler : public Resource
	{
		RTTI_DECLARATIONS_ABSTRACT(Sampler, Resource)

	public:
		enum class Type
		{
			Undefined = -1,

			TrilinearWrap,
			TrilinearMirror,
			TrilinearClamp,
			TrilinearBorder,
			PointClamp,
			DepthMap,
			ShadowMap,
			PcfShadowMap,
		};
		
	protected:
		Sampler() = default;

	public:
		virtual ~Sampler() = default;
		Sampler(const Sampler&) = default;
		Sampler& operator=(const Sampler&) = default;
		Sampler(Sampler&&) = default;
		Sampler& operator=(Sampler&&) = default;

	public:
		virtual Resource::Type ResourceType() const override final { return Resource::Type::Sampler; }
	};
}
