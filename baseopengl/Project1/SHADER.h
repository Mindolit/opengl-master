#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

class Shader {
	
public:
	unsigned int ID;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		std::string vertexShaderCode;
		std::string fragmentShaderCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		//시작
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vertexShaderFile.open(vertexShaderPath);
			fragmentShaderFile.open(fragmentShaderPath);

			std::stringstream vertexStream, fragStream;
			vertexStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();
			fragStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();

			vertexShaderCode = vertexStream.str();
			fragmentShaderCode = fragStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Shader.h:: 파일 읽기 실패\n";
		}
		//파일 읽기 성공, 쉐이더 컴파일 시작
		const char* vShaderCode = vertexShaderCode.c_str();
		const char* fShaderCode = fragmentShaderCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Vertex 쉐이더 컴파일 실패:" << infoLog << std::endl;
		}
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Fragment 쉐이더 컴파일 실패:" << infoLog << std::endl;
		}
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "쉐이더 프로그램 링킹 에러:" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use()
	{
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const 
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat4(const std::string& name, float Red,float Green,float Blue,float Alpha)const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()),Red,Green,Blue,Alpha);
	}
	void setMat4(const std::string& name, glm::mat4 mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE,glm::value_ptr(mat));
	}
	void setVec3(const std::string& name, float x,float y,float z)const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()),x,y,z);
	}
	void setVec3(const std::string& name, glm::vec3 vec) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), vec.x,vec.y,vec.z);
	}
};