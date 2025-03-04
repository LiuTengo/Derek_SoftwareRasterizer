#include "../Material.h"

Material::Material(Shader* shader)
{
	this->shader.reset(shader);
}

Material::~Material()
{
}

FragmentInput Material::ApplyVertexShader(const VertexInput& input)
{
	return shader->VertexShader(input);
}

Vector3f Material::ApplyFragmentShader(FragmentInput& input)
{
	input.SetTexture(texture);
	return shader->FragmentShader(input);
}

void Material::SetTexture(Texture* texture)
{
	this->texture.reset(texture);
}
