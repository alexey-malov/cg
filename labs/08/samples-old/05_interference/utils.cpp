#include "StdAfx.h"
#include "utils.h"

std::string LoadTextFile(const char fileName[])
{
	FILE * pFile = fopen(fileName, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		size_t length = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char * buf = new char[length];
		fread(buf, 1, length, pFile);
		fclose(pFile);
		std::string shaderSource(buf, length);;
		delete [] buf;
		return shaderSource;
	}
	throw std::runtime_error(std::string("Cannot open file ") + fileName);
}

void WriteLog(std::string const& str)
{
	printf("%s", str.c_str());
}

void LoadShaderSource(GLhandleARB shader, const char fileName[])
{
	WriteLog(std::string("loading shader ") + fileName + std::string("\n"));
	std::string shaderSrc = LoadTextFile(fileName);
	if (!shaderSrc.empty())
	{
		const char* shaderstring = shaderSrc.c_str();
		const char** pshaderstring = &shaderstring;
		glShaderSourceARB(shader, 1, pshaderstring, NULL);
		WriteLog("Ok\n");
	}
	else
	{
		throw std::runtime_error(std::string("Cannot load shader from file ") + fileName);
	}
}

std::string GetInfoLog(GLhandleARB object)
{
	if (object != NULL)
	{
		GLcharARB buffer[10000];
		GLsizei length;
		glGetInfoLogARB(object, sizeof(buffer) - 1, &length, buffer);
		return std::string(buffer, length);
	}
	else
	{
		throw std::logic_error("null pointer object is passed to GetInfoLog()");
	}
}

void CompileShader(GLhandleARB shader)
{
	glCompileShaderARB(shader);
	GLint compileStatus;
	glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus);
	bool result = (compileStatus == GL_TRUE);
	
	if (!result)
	{
		throw std::runtime_error(GetInfoLog(shader));
	}
}

void LinkProgram(GLhandleARB program)
{
	glLinkProgramARB(program);
	GLint linkStatus;
	glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &linkStatus);
	
	if (!linkStatus)
	{
		throw std::runtime_error(GetInfoLog(program));
	}
}
