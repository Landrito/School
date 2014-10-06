#include <fstream>
#include <string.h>
#include "ShaderLoader.h"

const int STR_LEN = 256;

ShaderLoader::ShaderLoader(const int size)
{
	 shaderText = new char[size];
}

ShaderLoader::~ShaderLoader()
{
    delete [] shaderText;
}

void ShaderLoader::load(const char * fName)
{
    char * input = new char[1000];

    for(int i = 0; i < 1000; i++)
    {
	shaderText[i] = '\0';
	input[i] = '\0';
    }

    std::ifstream shaderFile(fName);

    do
    {
       shaderFile.getline(input, STR_LEN);
       strcat(shaderText, input); 
       shaderText[strlen(shaderText)] = '\n';
    }
    while( shaderFile.good() );
}

const char * ShaderLoader::getShader()
{
	return shaderText;
}

