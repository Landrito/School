class ShaderLoader
{
public:
	ShaderLoader(const int size = 1000);
	~ShaderLoader();
	void load(const char * fName);
	const char * getShader();

private:
	char * shaderText;
};