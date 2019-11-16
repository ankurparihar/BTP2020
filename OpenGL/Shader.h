#pragma once

#include "Common.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderProgramSource ParseShader(const std::string& filePath);

unsigned int CompileShader(unsigned int type, const std::string& source);

int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);