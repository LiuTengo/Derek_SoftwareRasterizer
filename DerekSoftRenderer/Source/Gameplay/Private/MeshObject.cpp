#include "../MeshObject.h"

MeshObject::MeshObject(const std::string& filePath, Material* mat)
	:Object()
{
	objl::Loader loader;
	if (loader.LoadFile(filePath)) {
		auto mesh = loader.LoadedMeshes[0];
		
		triangleList.resize(mesh.Vertices.size() / 3);

		for (int i = 0; i < mesh.Vertices.size(); i += 3) {
			Triangle* t = new Triangle();
			for (int j = 0; j < 3; j++) {
				auto v = mesh.Vertices[i + j];
				t->SetVerticesData(j,
					Vector4f{ v.Position.X , v.Position.Y , v.Position.Z ,1 },
					Vector3f{ v.Normal.X , v.Normal.Y , v.Normal.Z },
					Vector3f{ 0,0.5f,0 });
			}
			triangleList[i / 3] = t;
		}
	}
	else {
		//unable to load the target file
		OutputDebugString((L"Unable to load the .obj file\n"));
	}

	material.reset(mat);
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
	float sa = sin(rotation.x());
	float sb = sin(rotation.y());
	float sc = sin(rotation.z());
	float ca = cos(rotation.x());
	float cb = cos(rotation.y());
	float cc = cos(rotation.z());
	//Notie: 可能会出错?
	//Fixme: 使用四元数旋转
	Matrix4X4f rotationM = {
		{cc * cb,cc * sb * sa - sc * ca,sc * sa + cc * sb * ca,0},
		{sc * cb,cc * ca + sb * sc * sa,sc * sb * ca - cc * sa,0},
		{-sb,cb * sa,cb * ca,0},
		{0,0,0,1}
	};

	return translateM * rotationM * scaleM;
}

void MeshObject::SetMaterial(Material* mat)
{
	material.reset(mat);
}

void MeshObject::UseVertexShader()
{
	if (material != nullptr) {
		material->ApplyVertexShader();
	}
	else {
		//Debug
	}
}

Vector3f MeshObject::UseFragmentShader()
{
	if (material != nullptr) {
		return material->ApplyFragmentShader();
	}
	else {
		//Debug
		return Vector3f();
	}
}

void MeshObject::SetMVPMatrix()
{
}

void MeshObject::SetRenderer(std::shared_ptr<Renderer> r)
{
	renderer = r;
}

void MeshObject::SetupShaderInputData()
{
}
