#pragma once

#include <windows.h>
#include <iostream>

#include "Source/Rendering/Renderer.h"
#include "Source/Math/Matrix.hpp"
#include "Source/Math/Vector.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Renderer* renderer;


void Draw(const HDC& hdc,Renderer* renderer) {
    //双缓冲绘制
    
    //创建缓冲区
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT);
    SelectObject(mdc,bmp);
    //在缓冲区进行绘制
    renderer->Draw(mdc);
    //复制到DC
    BitBlt(hdc,0,0, RendererSettings::WINDOW_WIDTH, RendererSettings::WINDOW_HEIGHT,mdc,0,0,SRCCOPY);
    //释放缓冲区
    DeleteDC(mdc);
}

int main() {
    
    //std::vector<Triangle> triangles = std::vector<Triangle>(1, 
    //    Triangle(Vector3f{ 0.2f,0.0f,0.0f }, Vector3f{ 0.0f,0.3f,0.0f }, Vector3f{ 0.5f,0.5f,0.0f }));
    //std::cout << triangles[0].points[0] << std::endl;
    //std::cout << triangles[0].points[1] << std::endl;
    //std::cout << triangles[0].points[2] << std::endl;

    ////映射到屏幕坐标
    //Vector3f screenSize = Vector3f{ (float)RendererSettings::RENDER_TARGET_WIDTH , (float)RendererSettings::RENDER_TARGET_HEIGHT ,1 };
    //for (auto& t : triangles) {
    //    t.points[0] *= screenSize;
    //    t.points[1] *= screenSize;
    //    t.points[2] *= screenSize;

    //    std::cout << t.points[0] << std::endl;
    //    std::cout << t.points[1] << std::endl;
    //    std::cout << t.points[2] << std::endl;
    //}

    //Test
    Matrix4X4i mat1 = {
        {2,1,3,4},
        {0,0,0,0},
        {1,1,1,1},
        {1,2,3,4}
    };

    Matrix4X4i mat2 = {
        {2,0,0,0},
        {0,2,0,0},
        {0,0,2,0},
        {0,0,0,2}
    };
    Matrix4X4i mat;
    Matrix4X4i::PrintMatrix(mat);

    Vector3f vec3 = { 2,1,3 };
    Vector4f vec4 = vec3.expand();
    Matrix4X4f transformMat = {
        {1,0,0,-1},
        {0,1,0,0},
        {0,0,1,1},
        {0,0,0,1}
    };
    Vector4f vec = transformMat * vec4;
    std::cout << vec << std::endl;
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
    renderer = new Renderer();

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        //OutputDebugString((L"In Loop\n"));
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        //std::cout << "Close Window" << std::endl;
        OutputDebugString((L"Close Window\n"));
        return 0;
    //case WM_SIZE:
    //    RendererSettings::RENDER_TARGET_HEIGHT = HIWORD(lParam);
    //    RendererSettings::RENDER_TARGET_WIDTH = LOWORD(lParam);
    //    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.
        OutputDebugString((L"Paint Window\n"));
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        Draw(hdc,renderer);

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}