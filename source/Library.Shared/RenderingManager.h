#pragma once

#include "RTTI.h"

namespace Library
{
	// Forward Declarations
	class Mesh;
	
	class RenderingManager : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderingManager, RTTI)

	public:
		struct Device;
		struct IndexBuffer;
		
	public:
		using WindowCallback = std::shared_ptr<std::function<void* ()>>;
		
	public:
		virtual Device& GetDevice() = 0;
		virtual const Device& GetDevice() const = 0;
		virtual void SetDevice(Device device) = 0;
		
		virtual IndexBuffer CreateMeshIndexBuffer(const Mesh& mesh) const = 0;

		const WindowCallback& GetWindowCallback() const;
		
	private:
		WindowCallback mGetWindow;
	};
}

#include "RenderingManager.inl"