#pragma once
#include "../Shader.h"
#include "../RendererSettings.h"

class DefaultShader : public Shader
{
private:
	float Clamp(float val);
public:
	virtual FragmentInput VertexShader(const VertexInput& input) override;
	virtual Vector3f FragmentShader(const FragmentInput& input) override;
};

inline float DefaultShader::Clamp(float val)
{
	return min(max(val,0),1);
}

inline FragmentInput DefaultShader::VertexShader(const VertexInput& input) {
	FragmentInput o;
	o.clipPosition = RendererSettings::mvp * input.modelPosition;
	Vector4f temp = (RendererSettings::m * input.modelPosition);
	o.worldPosition = Vector3f{ temp.x(),temp.y(),temp.z()};
	Vector4f normTemp = input.normal.toVector4();
	Vector4f worldNorm = (RendererSettings::m * normTemp);
	o.worldNormal = worldNorm.toVector3();
	return o;
}

inline Vector3f DefaultShader::FragmentShader(const FragmentInput& input)
{
	Vector3f defCol = Vector3f{ 1.0f,0.6f,0.0f };
	if (input.baseColor != nullptr) {
		defCol = input.baseColor->Sample(input.uv);
	}
	else {
		defCol = Vector3f{ 1.0f,1.0f,1.0f };
	}
	
	Vector3f Normal = Vector3f::normalize(input.worldNormal);
	Vector3f lightDir = Vector3f::normalize(RendererSettings::LightPosition.toVector3() - input.worldPosition);
	float halfLambert = (Normal.dot(lightDir)+1.0)*0.5;

	//diffuse
	Vector3f diffuse = halfLambert* RendererSettings::LightIntensity*(RendererSettings::LightColor)*defCol;
	//ambient
	Vector3f ambient = Vector3f{ 0.05f, 0.05f, 0.05f };
	//specular
	Vector3f halfDirection = Vector3f::normalize((RendererSettings::CameraDirection + lightDir));

	Vector3f specular = (pow(Clamp(halfDirection.dot(Normal)), 50) * RendererSettings::LightIntensity)*RendererSettings::LightColor;

	Vector3f res = diffuse + specular + ambient;
	return res;
}