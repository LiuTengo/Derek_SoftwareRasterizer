#include "../MeshObject.h"

MeshObject::MeshObject(Material* mat)
{
	LoadModelFromObj();
	material.reset(mat);

	position = Vector3f{ 0, 0, 0 };
	rotation = Vector3f{ 0, 0, 0 };
	scale = Vector3f{ 1, 1, 1 };
}

MeshObject::MeshObject(const std::string& filePath, Material* mat)
	:Object()
{
	LoadModelFromObj(filePath);
	material.reset(mat);

	position = Vector3f{ 0, 0, 0 };
	rotation = Vector3f{ 0, 0, 0 };
	scale = Vector3f{ 1, 1, 1 };
}

MeshObject::~MeshObject()
{
	for(auto& t : triangleList) {
		if (t != nullptr) {
			delete t;
			t = nullptr;
		}
	}
}

void MeshObject::Update(float dt)
{

}

void MeshObject::SetLocation(const Vector3f& newPos)
{
	position = newPos;
}

void MeshObject::SetRotation(const Vector3f& newRotation)
{
	rotation = newRotation;
}

void MeshObject::SetScale(const Vector3f& newScale)
{
	scale = newScale;
}

void MeshObject::Translate(const Vector3f& offset)
{
	position = position + offset;
}

Matrix4X4f MeshObject::GetModelMatrix() const
{
	Matrix4X4f translateM = {
		{1,0,0,position.x()},
		{0,1,0,position.y()},
		{0,0,1,position.z()},
		{0,0,0,1}
	};
	Matrix4X4f scaleM = {
		{scale.x(),0,0,0},
		{0,scale.y(),0,0},
		{0,0,scale.z(),0},
		{0,0,0,1}
	};
	Vector3f ratio = PI * rotation / 180.0f;
	float sa = sin(ratio.x());
	float sb = sin(ratio.y());
	float sc = sin(ratio.z());
	float ca = cos(ratio.x());
	float cb = cos(ratio.y());
	float cc = cos(ratio.z());
	Matrix4X4f rotationM = {
		{cc * cb,cc * sb * sa - sc * ca,sc * sa + cc * sb * ca,0},
		{sc * cb,cc * ca + sb * sc * sa,sc * sb * ca - cc * sa,0},
		{-sb,cb * sa,cb * ca,0},
		{0,0,0,1}
	};

	return   scaleM * rotationM * translateM;
}

void MeshObject::SetMaterial(Material* mat)
{
	material.reset(mat);
}

FragmentInput MeshObject::UseVertexShaderProgram(const VertexInput& vertInput)
{
	if (material != nullptr) {
		return material->ApplyVertexShader(vertInput);
	}
	else {
		//Debug
		return FragmentInput();
	}
}

Vector3f MeshObject::UseFragmentShaderProgram(FragmentInput& fragInput)
{
	if (material != nullptr) {
		return material->ApplyFragmentShader(fragInput);
	}
	else {
		//Debug
		return Vector3f{0,0,0};
	}
}

void MeshObject::SetMVPMatrix()
{
	return ;
}

void MeshObject::SetupShaderInputData()
{
}

void MeshObject::LoadModelFromObj(const std::string& filePath)
{
	objl::Loader loader;
	if (loader.LoadFile(filePath)) {
		auto mesh = loader.LoadedMeshes[0];

		for (int i = 0; i < mesh.Indices.size()-2; i += 3) {
			Triangle* t = new Triangle();
			for (int j = 0; j < 3; j++) {
				auto v = mesh.Vertices[mesh.Indices[i + j]];
				t->SetVerticesData(j,
					Vector4f{ v.Position.X , v.Position.Y , v.Position.Z ,1 },//模型是右手坐标系
					Vector3f{ v.Normal.X , v.Normal.Y , v.Normal.Z },
					Vector2f{ v.TextureCoordinate.X,v.TextureCoordinate.Y });
			}
			triangleList.push_back(t);
		}
	}
	else {
		//unable to load the target file
		OutputDebugString((L"Unable to load the .obj file\n"));
	}
}
