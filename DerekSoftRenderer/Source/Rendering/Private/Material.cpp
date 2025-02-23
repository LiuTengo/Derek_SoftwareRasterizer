#include "../Material.h"

Material::Material(const Shader*& shader)
{
	this->shader = std::make_shared<Shader>(shader);
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
