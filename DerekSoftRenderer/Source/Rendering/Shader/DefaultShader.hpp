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
	Vector4f normTemp = Vector4f{ input.normal.x(),input.normal.y(),input.normal.z(),0};
	Vector4f worldNorm = (RendererSettings::m * normTemp);
	o.worldNormal = Vector3f::normalize(worldNorm.toVector3());
	return o;
}

inline Vector3f DefaultShader::FragmentShader(const FragmentInput& input)
{
	Vector3f defCol;
	if (input.baseColor != nullptr) {
		defCol = input.baseColor->Sample(input.uv);
	}
	else {
		defCol = Vector3f{ 1.0f,1.0f,1.0f };
	}
	
	Vector3f Normal = Vector3f::normalize(input.worldNormal);
	Vector3f lightDir = Vector3f::normalize(RendererSettings::LightPosition - input.worldPosition);
	float halfLambert = (Normal.dot(lightDir)+1.0)*0.5;

	//diffuse
	Vector3f diffuse = halfLambert* RendererSettings::LightIntensity*(RendererSettings::LightColor)*defCol;
	//ambient
	Vector3f ambient = Vector3f{ 0.05f, 0.05f, 0.05f };
	//specular
	Vector3f viewDir = Vector3f::normalize(RendererSettings::CameraPosition - input.worldPosition);
	Vector3f halfDirection = Vector3f::normalize((viewDir + lightDir));

	Vector3f specular = (pow(Clamp(halfDirection.dot(Normal)), 40) * RendererSettings::LightIntensity)*RendererSettings::LightColor;



	//add shadow
	Matrix4X4f lightViewPort = Matrix4X4f{
		{(float)RendererSettings::ShadowMapWidth *0.5f,0,0,(float)RendererSettings::ShadowMapWidth * 0.5f},
		{0,(float)RendererSettings::ShadowMapHeight * 0.5f,0,(float)RendererSettings::ShadowMapHeight * 0.5f},
		{0,0,1,0},
		{0,0,0,1}
	};
	Vector4f lightCoord = RendererSettings::GetLightCoordinate(input.worldPosition);
	lightCoord = lightViewPort * lightCoord;
	//lightCoord.z() = RendererSettings::LightDepth((double)lightCoord.z());

	// 转换到阴影贴图坐标
	int x = floor(lightCoord.x());
	int y = floor(lightCoord.y());
	float testDepth = 0.0f;
	if (0 <= x && x < RendererSettings::ShadowMapWidth &&
		0 <= y && y < RendererSettings::ShadowMapHeight) {
		int index = //x + (RendererSettings::ShadowMapHeight - (int)lightCoord.y() -1)* RendererSettings::ShadowMapWidth;
		y * RendererSettings::ShadowMapWidth + x;

		double shadowDepth = RendererSettings::ShadowMap[index];
		if (lightCoord.z() > shadowDepth) {
			diffuse *= 0.5f;
			testDepth = 0.5f;
		}
	}
	
	Vector3f res =  Vector3f{ (float)testDepth };//diffuse + specular + ambient;
	return res;
}