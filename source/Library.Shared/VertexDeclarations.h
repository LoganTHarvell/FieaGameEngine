#pragma once

#pragma region Includes
// Third Party
#include <gsl/gsl>

// First Party
#include "RenderingManager.h"
#include "Buffer.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	class Mesh;

	/**
	 * @brief Base class representing basic vertex types
	 * @tparam T Derived VertexDeclaration class type
	*/
	template <typename T>
	struct VertexDeclaration
	{
		/**
		 * @brief Gets the size  data
		 * @return Size  data in bytes
		*/
		static constexpr std::uint32_t VertexSize() { return gsl::narrow_cast<std::uint32_t>(sizeof(T)); }

		/**
		 * @brief Gets the size required for the vertex buffer with the vertex declaration type @paramref T
		 * @param vertexCount Number of vertices in the vertex buffer
		 * @return Size  buffer in bytes 
		*/
		static constexpr std::uint32_t VertexBufferByteWidth(const std::size_t vertexCount) { return gsl::narrow_cast<std::uint32_t>(sizeof(T) * vertexCount); }

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const T>& vertices);
	};

	/**
	 * @brief Represents a vertex declaration with position data
	*/
	class VertexPosition : public VertexDeclaration<VertexPosition>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements { _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPosition>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		 * @param position Position vector 
		*/
		explicit VertexPosition(const glm::vec4& position=glm::vec4(0, 0, 0, 1)) :
			Position(position) { }
#pragma endregion Special Members

	public:
		/**
		 * @brief Position vector 
		*/
		glm::vec4 Position;
	};

	/**
	 * @brief Represents a vertex with position and color data
	*/
	class VertexPositionColor : public VertexDeclaration<VertexPositionColor>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("COLOR", 0, Format::RGBA32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionColor>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionColor() = default;
		
		/**
		 * @brief Specialized constructor for initializing the vertex data
		 * @param position Position vector 
		 * @param color Color 
		*/
		VertexPositionColor(const glm::vec4& position, const glm::vec4& color) :
			Position(position), Color(color) { }
#pragma endregion Special Members

	public:
		glm::vec4 Position{ 0,0,0,1 };	//!< Position vector 
		glm::vec4 Color{ 0 };					//!< Color 
	};

	/**
	 * @brief Represents a vertex with position and texture coordinate data
	*/
	class VertexPositionTexture : public VertexDeclaration<VertexPositionTexture>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("TEXCOORD", 0, Format::RG32Float)
		};
	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionTexture>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionTexture() = default;
		
		/**
		 * @brief Specialized constructor for initializing the vertex data
		 * @param position Position vector 
		 * @param textureCoordinates UV coordinates 
		*/
		VertexPositionTexture(const glm::vec4& position, const glm::vec2& textureCoordinates) :
			Position(position), TextureCoordinates(textureCoordinates) { }
#pragma endregion Special Members

		glm::vec4 Position{ 0,0,0,1 };	//!< Position vector 
		glm::vec2 TextureCoordinates{ 0 };	//!< UV coordinates 
	};

	/**
	 * @brief Represents a vertex with position and size data
	*/
	class VertexPositionSize : public VertexDeclaration<VertexPositionSize>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("SIZE", 0, Format::RG32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionSize>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionSize() = default;

		/**
		 * @brief Specialized constructor for initializing the vertex
		 * @param position Position vector 
		 * @param size Size  (width and height)
		*/
		VertexPositionSize(const glm::vec4 & position, const glm::vec2 & size) :
			Position(position), Size(size) { }
#pragma endregion Special Members

	public:
		glm::vec4 Position{ 0,0,0,1 };	//!< Position vector 
		glm::vec2 Size{ 0, 0 };					//!< Size  (width and height)
	};

	/**
	 * @brief Represents a vertex with position and normal vector data
	*/
	class VertexPositionNormal : public VertexDeclaration<VertexPositionNormal>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("NORMAL", 0, Format::RGB32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionNormal>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionNormal() = default;

		/**
		 * @brief Specialized constructor for initializing vertex data
		 * @param position Position vector 
		 * @param normal Normal vector 
		*/
		VertexPositionNormal(const glm::vec4 & position, const glm::vec3 & normal) :
			Position(position), Normal(normal) { }
#pragma endregion Special Members

	public:
		glm::vec4 Position{ 0,0,0,1 };	//!< Position vector 
		glm::vec3 Normal{ 0 };			//!< Normal vector 
	};

	/**
	 * @brief Represents a vertex with position, texture coordinate, and normal vector data
	*/
	class VertexPositionTextureNormal : public VertexDeclaration<VertexPositionTextureNormal>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("TEXCOORD", 0, Format::RG32Float),
			VertexAttribute("NORMAL", 0, Format::RGB32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionTextureNormal>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionTextureNormal() = default;

		/**
		 * @brief Specialized constructor for initializing vertex data
		 * @param position Position vector 
		 * @param textureCoordinates UV coordinates 
		 * @param normal Normal vector 
		*/
		VertexPositionTextureNormal(const glm::vec4 & position, const glm::vec2 & textureCoordinates, const glm::vec3 & normal) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }
#pragma endregion Special Members

	public:
		glm::vec4 Position{ 0,0,0,1 };		//!< Position vector 
		glm::vec2 TextureCoordinates{ 0 };	//!< UV coordinates 
		glm::vec3 Normal{ 0 };				//!< Normal vector 

	};

	/**
	 * @brief Represents a vertex with position, texture coordinate, normal, and tangent vector data
	*/
	class VertexPositionTextureNormalTangent : public VertexDeclaration<VertexPositionTextureNormalTangent>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("TEXCOORD", 0, Format::RG32Float),
			VertexAttribute("NORMAL", 0, Format::RGB32Float),
			VertexAttribute("TANGENT", 0, Format::RGB32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexPositionTextureNormalTangent>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexPositionTextureNormalTangent() = default;

		/**
		 * @brief Specialized constructor for initializing the vertex data
		 * @param position Position vector
		 * @param textureCoordinates UV coordinates
		 * @param normal Normal vector
		 * @param tangent Tangent vector
		*/
		VertexPositionTextureNormalTangent(const glm::vec4 & position, const glm::vec2 & textureCoordinates, const glm::vec3 & normal, const glm::vec3 & tangent) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent) { }
#pragma endregion Special Members
		
	public:
		glm::vec4 Position{ 0,0,0,1 };		//!< Position vector 
		glm::vec2 TextureCoordinates{ 0 };	//!< UV coordinates 
		glm::vec3 Normal{ 0 };				//!< Normal vector 
		glm::vec3 Tangent{ 0 };				//!< Tangent vector 
	};

	/**
	 * @brief Represents a vertex with bone index and weight data and position, texture coordinate, and normal vector data
	*/
	class VertexSkinnedPositionTextureNormal : public VertexDeclaration<VertexSkinnedPositionTextureNormal>
	{
#pragma region Static Members
	private:
		/**
		 * @brief Vertex attribute data associated with the vertex declaration
		*/
		inline static const VertexAttribute _InputElements[]
		{
			VertexAttribute("POSITION", 0, Format::RGBA32Float),
			VertexAttribute("TEXCOORD", 0, Format::RG32Float),
			VertexAttribute("NORMAL", 0, Format::RGB32Float),
			VertexAttribute("BONEINDICES", 0, Format::RGBA32Float),
			VertexAttribute("BONEWEIGHTS", 0, Format::RGBA32Float)
		};

	public:
		/**
		 * @brief Handle to the vertex attribute data associated with the vertex declaration
		*/
		inline static const gsl::span<const VertexAttribute> InputElements{ _InputElements };

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param mesh Mesh data used to initialize the vertex buffer
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const Library::Mesh& mesh);

		/**
		 * @brief Creates a vertex buffer from the given vertex data
		 * @param renderingManager RenderingManager instance used to create the buffer
		 * @param vertices Vector of Vertex data
		 * @return Pointer to the created vertex buffer
		*/
		static Buffer* CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const VertexSkinnedPositionTextureNormal>& vertices)
		{
			return VertexDeclaration::CreateVertexBuffer(renderingManager, vertices);
		}
#pragma endregion Static Members

#pragma region Special Members
	public:
		/**
		 * @brief Default constructor
		*/
		VertexSkinnedPositionTextureNormal() = default;

		/**
		 * @brief Specialized constructor for initializing the vertex data
		 * @param position Position of the vertex
		 * @param textureCoordinates UV coordinates
		 * @param normal Normal vector
		 * @param boneIndices Bone index list
		 * @param boneWeights Bone weight list
		*/
		VertexSkinnedPositionTextureNormal(const glm::vec4 & position, const glm::vec2 & textureCoordinates, const glm::vec3 & normal, const glm::uvec4 & boneIndices, const glm::vec4 & boneWeights) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), BoneIndices(boneIndices), BoneWeights(boneWeights) { }
#pragma endregion Special Members

	public:
		glm::vec4 Position;
		glm::vec2 TextureCoordinates;
		glm::vec3 Normal;
		glm::uvec4 BoneIndices;
		glm::vec4 BoneWeights;
	};
}

#include "VertexDeclarations.inl"