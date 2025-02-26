#include "../Material.h"

Material::Material(Shader s)
{
	this->shader = std::make_shared<Shader>(s);
}

Material::~Material()
{
}

//接收CPU读取的文件数据，转换成VS的顶点数据
void Material::SetVertexInputData(const Triangle& triangle)
{
	
}

void Material::ApplyVertexShader()
{
	fragInput = shader->VertexShader(vertInput);
}

Vector3f Material::ApplyFragmentShader()
{
	return shader->FragmentShader(fragInput);
}
