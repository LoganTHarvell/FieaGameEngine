#pragma once
#include <GL/gl3w.h>
namespace GameDesktop{
	/// <summary>
	/// Represents a OpenGL quad mesh
	/// </summary>
	class Quad
	{
	public:
		Quad() = default;
		Quad(const Quad&) = default;
		Quad(Quad&&) = default;
		Quad& operator=(const Quad&) = default;
		Quad& operator=(Quad&&) = default;
		~Quad();

		/// <summary>
		/// Creates the quad
		/// </summary>
		void Initialize();
		/// <summary>
		/// Renders the quad with current texture
		/// </summary>
		void Render() const;
		void UpdateUV(unsigned int numColumns, unsigned int numRows, size_t columnIndex, size_t rowIndex) const;
		/// <summary>
		/// Deletes the butter of this mesh
		/// </summary>
		void Clear();
	private:
		inline static const GLsizei sIndexCount = 6;
		inline static const GLfloat sVertices[] =
		{	//x      y     z     u     v
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom right
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // upper right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // upper left
		};
		inline static const size_t sElements[] =
		{
			0,1,2,
			2,3,0
		};

		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
	};
}

