#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "../Math/Vector.hpp"

#pragma comment(lib, "gdiplus.lib")

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture() = default;

public:
	int width;
	int height;
	std::vector<Vector3f> image;
	 
	Vector3f Sample(const Vector2f& uv);
private:
	bool LoadPngImage(const wchar_t* filename);
	float Clamp(float val);
	WCHAR* StringToWCHAR(const std::string& str);
	void SetImageColor(HBITMAP hbmp);
};

