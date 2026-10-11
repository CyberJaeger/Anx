#include "windows.hpp"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

Windows::Windows() {
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTONEAREST));

    WNDCLASSEXW wc = {
        sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L,
        GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
        L"AnxWindowClass", nullptr
    };
    ::RegisterClassExW(&wc);

    m_hWnd = ::CreateWindowW(
        wc.lpszClassName, L"Anx - DirectX 11",
        WS_OVERLAPPEDWINDOW, 100, 100,
        (int)(1280 * main_scale), (int)(800 * main_scale),
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!CreateDevices(m_hWnd)) {
        CleanupDevices();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hWnd);
}

Windows::~Windows() {
    CleanupDevices();
    if (m_hWnd) {
        ::DestroyWindow(m_hWnd);
    }
}

bool Windows::CreateDevices(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    sd.BufferCount = 2;

    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pContext);

    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pContext);

    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void Windows::CleanupDevices() {
    ClearRenderTarget();
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
    if (m_pContext) { m_pContext->Release(); m_pContext = nullptr; }
    if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }
}

void Windows::CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
        pBackBuffer->Release();
    }
}

void Windows::ClearRenderTarget() {
    if (m_pRenderTargetView) { m_pRenderTargetView->Release(); m_pRenderTargetView = nullptr; }
}

void Windows::CreateSwapChain(HWND hWnd, UINT width, UINT height) {
    if (m_pSwapChain) {
        ClearRenderTarget();
        m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }
}

bool Windows::ProcessMessages() {
    MSG msg = {};
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            return false; 
    }
    return true;
}

void Windows::BeginFrame() {
   
    const float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
    m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
    m_pContext->ClearRenderTargetView(m_pRenderTargetView, clear_color);
}

void Windows::EndFrame() {
    HRESULT hr = m_pSwapChain->Present(1, 0);
    g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}
