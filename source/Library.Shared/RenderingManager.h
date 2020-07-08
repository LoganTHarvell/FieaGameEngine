#pragma once

#pragma region Includes
// First Party
#include "RTTI.h"
#include "RenderingTypes.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	class RenderContext;
	class Buffer;
	class Mesh;

	/**
	 * @brief Abstraction of the main interface for the rendering system.
	 *		  It is the only class directly used by the engine.
	 *		  Relies on rendering API implementations for actual rendering.
	*/
	class RenderingManager : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderingManager, RTTI)

#pragma region Type Definitions
	public:
		/**
		 * @brief Specifier for the rendering API implementation in use.
		*/
		enum class API
		{
			None,

			OpenGL,
			Direct3D11
		};
		
		using WindowCallback = std::shared_ptr<std::function<void* ()>>;
#pragma endregion Type Definitions

#pragma region Special Members
	protected:
		/**
		 * @brief Default Constructor
		 * @param api API specifier to be passed by a subclass.
		*/
		explicit RenderingManager(const API api) : mAPI(api) {}

		/* Default Special Members */
		virtual ~RenderingManager() = default;
		RenderingManager(const RenderingManager & rhs) = default;
		RenderingManager& operator=(const RenderingManager & rhs) = default;
		RenderingManager(RenderingManager && rhs) = default;
		RenderingManager& operator=(RenderingManager && rhs) = default;
#pragma endregion Special Members

#pragma region Modifiers
	public:
		/**
		 * @brief Creates a render context.
		 * @return Pointer to the render context created.
		*/
		virtual RenderContext* CreateRenderContext() = 0;

		/**
		 * @brief Creates an index buffer for a given mesh and returns it.
		 * @param mesh Mesh used to create the index buffer.
		 * @return Pointer to the Buffer that was created.
		*/
		virtual Buffer* CreateMeshIndexBuffer(const Mesh& mesh) = 0;

		/**
		 * @brief Sets the primitive topology of the render device context.
		 * @param topology Topology type to be set.
		*/
		virtual void SetPrimitiveTopology(const PrimitiveTopology& topology) = 0;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer.
		 * @param buffer Index buffer to be used.
		*/
		virtual void SetIndexBuffer(Buffer& buffer) = 0;
		
		//virtual void SetVertexBuffer(Buffer& buffer) = 0;
#pragma endregion Modifiers

#pragma region Accessors
	public:
		/**
		 * @brief Gets the rendering API specifier.
		 * @return 
		*/
		API GetAPI() const;

		/**
		 * @brief Gets the window callback functor.
		 *		  Functor is called on every window event.
		 *		  
		 * @return Window callback functor.
		*/
		const WindowCallback& GetWindowCallback() const;
#pragma endregion Accessors

#pragma region Data Members
	private:
		API mAPI;					//!< @brief Specifier denoting the actual rendering API implementation in use.
		WindowCallback mGetWindow;	//!< @brief Window callback functor.
#pragma endregion Data Members
	};
}

#include "RenderingManager.inl"