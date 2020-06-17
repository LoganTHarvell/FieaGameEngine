#include "pch.h"
#include "Shader.h"

namespace GameDesktop
{
	Shader::~Shader()
	{
		ClearShader();
	}
	void Shader::CreateFromFiles(const std::string& vertexLocation, const std::string& fragmentLocation)
	{
		CompileShader(ReadFile(vertexLocation), ReadFile(fragmentLocation));
	}
	const GLuint Shader::GetProjectionLocation() const
	{
		return mUniformProj;
	}
	const GLuint Shader::GetModelLocation() const
	{
		return mUniformModel;
	}
	void Shader::UseShader() const
	{
		glUseProgram(mShaderId);
	}
	void Shader::ClearShader()
	{
		if (mShaderId != 0)
		{
			glDeleteProgram(mShaderId);
			mShaderId = 0;
		}
		mUniformModel = 0;
		mUniformProj = 0;
	}
	void Shader::CompileShader(const std::string& vertexCode, const std::string& fragmentCode)
	{
		mShaderId = glCreateProgram();
		if (!mShaderId)
		{
			throw std::exception("Shader program creation failed!");
		}
		AddShader(mShaderId, vertexCode, GL_VERTEX_SHADER);
		AddShader(mShaderId, fragmentCode, GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		glLinkProgram(mShaderId);
		glGetProgramiv(mShaderId, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(mShaderId, sizeof(eLog), NULL, eLog);
			std::cout << "Error linking program: " << eLog << std::endl;
			throw std::exception("Program linking failed!");
		}

		glValidateProgram(mShaderId);
		glGetShaderiv(mShaderId, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(mShaderId, sizeof(eLog), NULL, eLog);
			std::cout << "Error validating program: " << eLog << std::endl;
			throw std::exception("Program validation failed!");
		}

		mUniformModel = glGetUniformLocation(mShaderId, "model");
		mUniformProj = glGetUniformLocation(mShaderId, "projection");
	}
	void Shader::AddShader(GLuint program, const std::string& shaderCode, GLenum shaderType)
	{
		GLuint shader = glCreateShader(shaderType);

		const GLchar* code = shaderCode.c_str();
		GLint codeLength = static_cast<GLint>(shaderCode.size());
		
		glShaderSource(shader, 1, &code, &codeLength);
		glCompileShader(shader);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
			std::cout << "Error compiling shader: " << eLog << std::endl;
			throw std::exception("Shader compilation failed");
		}

		glAttachShader(program, shader);
	}
	std::string Shader::ReadFile(const std::string& fileLocation)
	{
		std::ifstream fileStream(fileLocation);
		return std::string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	}
	void Shader::CreateFromString(const std::string& vertexCode, const std::string& fragmentCode)
	{
		CompileShader(vertexCode, fragmentCode);
	}
}