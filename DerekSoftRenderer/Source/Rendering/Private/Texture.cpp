#include "../Texture.h"

Texture::Texture(const std::string& filePath)
{
	LoadPngImage(StringToWCHAR(filePath));
}

Vector3f Texture::Sample(const Vector2f& uv)
{
	float x = Clamp(uv.data[0]);
	float y = Clamp(1-uv.data[1]);
	int u = (int)(width*x);
	int v = (int)(height*y);

	v = fmin(v, height - 1);

	return image[v*width+u];
}

bool Texture::LoadPngImage(const wchar_t* filename) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// 加载PNG图片
	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(filename);
	if (bitmap->GetLastStatus() != Gdiplus::Ok) {
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return false;
	}

	// 获取图片宽度和高度
	width = bitmap->GetWidth();
	height = bitmap->GetHeight();

	image.clear();

	// 遍历图片的每个像素并转换为Vector3
	for (UINT y = 0; y < height; ++y) {
		for (UINT x = 0; x < width; ++x) {
			Gdiplus::Color color;
			bitmap->GetPixel(x, y, &color);

			// 获取RGB值并转换为[0, 1]范围内的浮点数
			float r = color.GetRed() / 255.0f;
			float g = color.GetGreen() / 255.0f;
			float b = color.GetBlue() / 255.0f;

			// 将RGB值存入Vector3
			image.push_back(Vector3f{ r, g, b });
		}
	}

	delete bitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return true;
}

float Texture::Clamp(float val)
{
	return fmax(fmin(val,1),0);
}

WCHAR* Texture::StringToWCHAR(const std::string& str)
{
	// 计算需要的宽字符数组的大小
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (len == 0)
	{
		std::cerr << "Error in MultiByteToWideChar" << std::endl;
		return nullptr;
	}

	// 分配足够的内存来存储宽字符数组
	WCHAR* wstr = new WCHAR[len];

	// 执行转换
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);

	return wstr;
}

void Texture::SetImageColor(HBITMAP hbmp)
{
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;

	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP hDIB = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&image, NULL, 0);
	DeleteDC(hdc);

	if (hDIB)
	{
		// 将HBITMAP复制到DIB中
		HDC hdcMem = CreateCompatibleDC(NULL);
		SelectObject(hdcMem, hbmp);
		BitBlt(hdcMem, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);

		// 创建一个临时的字节数组来存储图像数据
		std::vector<BYTE> pixels(width * height * 3);  // 24-bit color (3 bytes per pixel)
		// 获取像素数据
		GetDIBits(hdc, hbmp, 0, height, pixels.data(), &bmi, DIB_RGB_COLORS);

		// 转换为Vector3f颜色数据
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int index = (y * width + x) * 3;  // 每个像素3字节
				BYTE* pixel = &pixels[index];

				// 假设我们以BGR顺序获取颜色
				float r = pixel[2] / 255.0f;
				float g = pixel[1] / 255.0f;
				float b = pixel[0] / 255.0f;

				// 将颜色存储为Vector3f对象
				image[y * width + x] = Vector3f{ r, g, b };
			}
		}
	}
}
