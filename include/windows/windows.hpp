#pragma once

#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>

class Windows {
public:
    Windows();
    ~Windows();

    bool ProcessMessages();
    void BeginFrame();
    void EndFrame();

    HWND GetHWND() const { return m_hWnd; }
    ID3D11Device* GetDevice() const { return m_pDevice; }
    ID3D11DeviceContext* GetContext() const { return m_pContext; }
    ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView; }
    IDXGISwapChain* GetSwapChain() const { return m_pSwapChain; }

    bool IsSwapChainOccluded() const { return g_SwapChainOccluded; }
    void SetSwapChainOccluded(bool occluded) { g_SwapChainOccluded = occluded; }

    UINT GetResizeWidth() const { return g_ResizeWidth; }
    UINT GetResizeHeight() const { return g_ResizeHeight; }
    void ResetResizeDimensions() { g_ResizeWidth = 0; g_ResizeHeight = 0; }

    void CreateSwapChain(HWND hWnd, UINT width, UINT height);

private:
    bool CreateDevices(HWND hWnd);
    void CleanupDevices();
    void CreateRenderTarget();
    void ClearRenderTarget();

    HWND m_hWnd = nullptr;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pContext = nullptr;
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;

    bool g_SwapChainOccluded = false;
    UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    UINT createDeviceFlags = 0;
};