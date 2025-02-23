#include "../Material.h"

Material::Material(const Shader*& shader)
{
	this->shader = std::make_shared<Shader>(shader);
}

Material::~Material()
{
}

//����CPU��ȡ���ļ����ݣ�ת����VS�Ķ�������
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
