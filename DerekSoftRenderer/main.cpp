#pragma once

#include <windows.h>
#include <WinUser.h>
#include <iostream>

#include "Source/SubClass/RotateObject.h"
#include "Source/SubClass/Plane.h"
#include "Source/Rendering/Shader/DefaultShader.hpp"
#include "Source/Rendering/Renderer.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::shared_ptr<Renderer> renderer;
Scene* scene;
BITMAPINFO bmi;

void InitData() {
    renderer = std::make_shared<Renderer>();
    scene = new Scene();

    Texture* spotText = new Texture(".//Content//Model//spot//spot_texture.png");

    Shader* simpleShader = new DefaultShader();
    Material* spot_mat = new Material(simpleShader);
    spot_mat->SetTexture(spotText);

    Material* plane_mat = new Material(simpleShader);

    Plane* plane = new Plane("./Content/Model/plane.obj", plane_mat);
    plane->SetScale(Vector3f{ 0.5f,0.5f,0.5f });
    plane->SetLocation(Vector3f{ 0.0f,-1.5f,0.0f });

    RotateObject* box = new RotateObject("./Content/Model/spot/spot_triangulated_good.obj",spot_mat);
    // "./Content/Model/box_stack.obj"
    // "./Content/Model/spot/spot_triangulated_good.obj"
    box->SetRotation(Vector3f{0.0f,145.0f,0.0f});

    Camera camera = Camera();
    Light light = Light();
    scene->SetMainCamera(camera);
    scene->SetMainLight(light);
    scene->AddObject(box);
    scene->AddObject(plane);

    bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = RendererSettings::WINDOW_WIDTH;
    bmi.bmiHeader.biHeight = -RendererSettings::WINDOW_HEIGHT; // 负值表示顶部为起点 (防止上下翻转)
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;  // 24-bit BGR
    bmi.bmiHeader.biCompression = BI_RGB;  // 无压缩
}

void OnCloseWindow() {
    delete scene;
    scene = nullptr;
}

void Draw(const HDC& hdc,const std::shared_ptr<Renderer>& renderer) {
    //双缓冲绘制
    //创建缓冲区
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT);
    SelectObject(mdc,bmp);

    scene->light->SetShadwMap(scene->objectArray, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT);
    //在缓冲区进行绘制
    renderer->Draw(mdc,scene->objectArray,scene->camera);
    //将每帧buffer写入HDC
    renderer->CopyFrameBufferToHDC(mdc);
    //StretchDIBits(mdc, 0, 0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,
    //   0, 0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,
    //    scene->light->ShadowMap.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);
    //复制到DC
    BitBlt(hdc,0,0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,mdc,0,0,SRCCOPY);
    //释放缓冲区
    DeleteObject(bmp);
    DeleteDC(mdc);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Derek Software Raterizer",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    InitData();

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_QUIT) { 
            return 0;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //设置计时器
        SetTimer(hwnd, 1, RendererSettings::TimeStep, NULL);

        srand((unsigned)time(NULL));
        break;
    }
    case WM_DESTROY:
        OnCloseWindow();
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    case WM_TIMER:
        if (scene != nullptr && wParam == 1)	// 进行更新
        {
            scene->Update((float)RendererSettings::TimeStep * 0.01f);
            InvalidateRect(hwnd, NULL, TRUE);	// 让窗口变为无效,从而触发重绘消息
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.
        OutputDebugString((L"Paint Window\n"));
        Draw(hdc,renderer);

        EndPaint(hwnd, &ps);
    }
    case WM_KEYDOWN: {
        switch (wParam) {
        case 'W':
            //OutputDebugString((L"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n"));
            break;
        case 'S':
            break;
        case 'A':
            break;
        case 'D':
            break;
        case VK_RBUTTON:
            break;
        }
        //InvalidateRect(hwnd, NULL, TRUE);
    }
    case WM_KEYUP: {
        switch (wParam) {
        case 'W':
            //OutputDebugString((L"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n"));
            break;
        case 'S':
            break;
        case 'A':
            break;
        case 'D':
            break;
        case VK_RBUTTON:
            break;
        }
        //InvalidateRect(hwnd, NULL, TRUE);
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//int main() {
//    
//    Vector3f vec{1,0,2};
//    std::cout << vec << std::endl;
//    vec = vec + Vector3f{0,1,2};
//    std::cout << vec << std::endl;
//    //Test
//    //Matrix4X4i mat1 = {
//    //    {2,1,3,4},
//    //    {0,0,0,0},
//    //    {1,1,1,1},
//    //    {1,2,3,4}
//    //};
//
//    //Matrix4X4i mat2 = {
//    //    {2,0,0,0},
//    //    {0,2,0,0},
//    //    {0,0,2,0},
//    //    {0,0,0,2}
//    //};
//    //Matrix4X4i mat;
//    //Matrix4X4i::PrintMatrix(mat);
//
//    //Vector3f vec3 = { 2,1,3 };
//    //Vector4f vec4 = vec3.expand();
//    //Matrix4X4f transformMat = {
//    //    {1,0,0,-1},
//    //    {0,1,0,0},
//    //    {0,0,1,1},
//    //    {0,0,0,1}
//    //};
//    //Vector4f vec = transformMat * vec4;
//    //std::cout << vec << std::endl;
//}