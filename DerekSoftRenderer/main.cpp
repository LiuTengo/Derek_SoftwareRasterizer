#pragma once

#include <windows.h>
#include <iostream>

#include "Source/Rendering/Shader/DefaultShader.hpp"
#include "Source/Rendering/Renderer.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::shared_ptr<Renderer> renderer;
Scene* scene;

void InitData() {
    renderer = std::make_shared<Renderer>();
    scene = new Scene();

    Texture* text = new Texture(".//Content//Model//spot//spot_texture.png");

    Shader* simpleShader = new DefaultShader();
    Material* box_mat = new Material(simpleShader);
    box_mat->SetTexture(text);

    MeshObject* box = new MeshObject("./Content/Model/spot/spot_triangulated_good.obj",box_mat);// "./Content/Model/box_stack.obj"
    
    //box->SetLocation(Vector3f{0.0,0.0,0.0f});
    box->SetRotation(Vector3f{0.0f,45.0f,0.0f});
    box->SetScale(Vector3f{2.f,2.f,2.f});

    Camera camera = Camera();
    Light light = Light();
    scene->SetMainCamera(camera);
    scene->SetMainLight(light);
    scene->AddObject(box);
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
    //在缓冲区进行绘制
    renderer->Draw(mdc,scene);
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
        L"Learn to Program Windows",    // Window text
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
    MSG msg = { };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
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
            scene->Update((float)RendererSettings::TimeStep * 0.001f);
            InvalidateRect(hwnd, NULL, TRUE);	// 让窗口变为无效,从而触发重绘消息
        }
        break;
    //case WM_SIZE:
    //    RendererSettings::RENDER_TARGET_HEIGHT = HIWORD(lParam);
    //    RendererSettings::RENDER_TARGET_WIDTH = LOWORD(lParam);
    //    break;
    case WM_ERASEBKGND:		// 不擦除背景,避免闪烁
        return 1;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.
        OutputDebugString((L"Paint Window\n"));
        Draw(hdc,renderer);

        EndPaint(hwnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    
    Vector3f vec{1,0,2};
    std::cout << vec << std::endl;
    vec = vec + Vector3f{0,1,2};
    std::cout << vec << std::endl;
    //Test
    //Matrix4X4i mat1 = {
    //    {2,1,3,4},
    //    {0,0,0,0},
    //    {1,1,1,1},
    //    {1,2,3,4}
    //};

    //Matrix4X4i mat2 = {
    //    {2,0,0,0},
    //    {0,2,0,0},
    //    {0,0,2,0},
    //    {0,0,0,2}
    //};
    //Matrix4X4i mat;
    //Matrix4X4i::PrintMatrix(mat);

    //Vector3f vec3 = { 2,1,3 };
    //Vector4f vec4 = vec3.expand();
    //Matrix4X4f transformMat = {
    //    {1,0,0,-1},
    //    {0,1,0,0},
    //    {0,0,1,1},
    //    {0,0,0,1}
    //};
    //Vector4f vec = transformMat * vec4;
    //std::cout << vec << std::endl;
}