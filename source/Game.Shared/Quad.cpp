#include "pch.h"
#include "Quad.h"

namespace GameDesktop
{
	Quad::~Quad()
	{
		Clear();
	}
	void Quad::Initialize()
	{
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sElements), sElements, GL_STATIC_DRAW);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sVertices), sVertices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
		glEnableVertexAttribArray(1);

		// Unbinding
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void Quad::Render() const
	{
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

		glDrawElements(GL_TRIANGLES, sIndexCount, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void Quad::UpdateUV(unsigned int numColumns, unsigned int numRows, size_t columnIndex, size_t rowIndex) const
	{
		if (numColumns == 0 || numRows == 0)
		{
			return;
		}
		if (columnIndex >= numColumns || rowIndex >= numRows)
		{
			throw std::out_of_range("Column or row index out of range!");
		}
		GLfloat bottomLeft[] = { 1.0f / float(numColumns) * columnIndex, 1.0f / float(numRows) * rowIndex };
		GLfloat bottomRight[] = { 1.0f / float(numColumns) * (columnIndex + 1), 1.0f / float(numRows) * rowIndex };
		GLfloat upperRight[] = { 1.0f / float(numColumns) * (columnIndex + 1), 1.0f / float(numRows) * (rowIndex + 1) };
		GLfloat upperLeft[] = { 1.0f / float(numColumns) * (columnIndex), 1.0f / float(numRows) * (rowIndex + 1) };
		glNamedBufferSubData(mVBO, 3 * sizeof(GLfloat), sizeof(bottomLeft), bottomLeft);
		glNamedBufferSubData(mVBO, 8 * sizeof(GLfloat), sizeof(bottomRight), bottomRight);
		glNamedBufferSubData(mVBO, 13 * sizeof(GLfloat), sizeof(upperRight), upperRight);
		glNamedBufferSubData(mVBO, 18 * sizeof(GLfloat), sizeof(upperLeft), upperLeft);
	}
	void Quad::Clear()
	{
		if (mIBO != 0)
		{
			// Deleting IBO from GPU
			glDeleteBuffers(1, &mIBO);
			mIBO = 0;
		}
		if (mVBO != 0)
		{
			glDeleteBuffers(1, &mVBO);
			mIBO = 0;
		}
		if (mVAO != 0)
		{
			glDeleteVertexArrays(1, &mVAO);
			mVAO = 0;
		}
	}
}