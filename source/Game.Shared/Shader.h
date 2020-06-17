#pragma once
#include <fstream>
#include <GL/gl3w.h>
#include <iostream>
#include <string>
#include <streambuf>

namespace GameDesktop
{
	class Shader
	{
	public:
		Shader() = default;
		~Shader();
		Shader(const Shader&) = default;
		Shader(Shader&&) = default;
		Shader& operator=(const Shader&) = default;
		Shader& operator=(Shader&&) = default;
		void CreateFromFiles(const std::string& vertexLocation, const std::string& fragmentLocation);

		const GLuint GetProjectionLocation() const;
		const GLuint GetModelLocation() const;

		void UseShader() const;
		void ClearShader();
	private:
		GLuint mShaderId = 0;
		GLuint mUniformProj = 0;
		GLuint mUniformModel = 0;

		void CompileShader(const std::string& vertexCode, const std::string& fragmentCode);
		void AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType);
		std::string ReadFile(const std::string& fileLocation);
		void CreateFromString(const std::string& vertexCode, const std::string& fragmentCode);
	};
}

