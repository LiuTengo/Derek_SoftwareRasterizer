#include "../Material.h"

Material::Material(Shader s)
{
	this->shader = std::make_shared<Shader>(s);
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
