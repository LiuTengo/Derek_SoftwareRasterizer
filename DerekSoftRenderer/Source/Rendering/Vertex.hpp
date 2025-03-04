#pragma once
#include "Texture.h"
#include "../Math/Vector.hpp"

struct Vertex {
	Vector4f clipPoints;
	Vector3f normal;
	Vector3f worldPosition;
	Vector2f uv;

	Vertex() {
		clipPoints = Vector4f{ 0,0,0,1 };
		normal = Vector3f{ 0,0,0 };
		uv = Vector2f{ 0,0 };
	}

	Vertex(Vector4f p, Vector3f norm) {
		clipPoints = Vector4f{ 0,0,0,1 };
		normal = Vector3f{ 0,0,0 };
		uv = Vector2f{ 0,0 };
	}
};

struct VertexInput {
	Vector4f modelPosition;
	Vector3f normal;
	Vector2f uv;

	VertexInput() = default;

	VertexInput(Vector4f pos, Vector3f norm, Vector2f _uv) {
		modelPosition = pos;
		normal = norm;
		uv = _uv;
	}

	VertexInput(const Vertex& vert) {
		modelPosition = vert.clipPoints;
		normal = vert.normal;
		uv = vert.uv;
	}
};

struct FragmentInput {
	Vector4f clipPosition;
	Vector3f worldPosition;
	Vector3f worldNormal;
	Vector2f uv;
	std::shared_ptr<Texture> texture;

	void SetTexture(std::shared_ptr<Texture> tex) {
		texture = tex;
	}
};