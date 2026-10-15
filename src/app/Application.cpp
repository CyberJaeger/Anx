#include "app/Application.hpp"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

Application::Application() {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    ImGui_ImplWin32_Init(m_windows.GetHWND());
    ImGui_ImplDX11_Init(m_windows.GetDevice(), m_windows.GetContext());
}

Application::~Application() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Application::Run() {
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (m_windows.ProcessMessages()) {

        if (m_windows.IsSwapChainOccluded() && m_windows.GetSwapChain()->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            ::Sleep(10);
            continue;
        }
        m_windows.SetSwapChainOccluded(false);

        if (m_windows.GetResizeWidth() != 0 && m_windows.GetResizeHeight() != 0) {
            m_windows.CreateSwapChain(m_windows.GetHWND(), m_windows.GetResizeWidth(), m_windows.GetResizeHeight());
            m_windows.ResetResizeDimensions();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();

        m_anxGui.Draw();

        ImGui::Render();
        m_windows.BeginFrame();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        m_windows.EndFrame();
    }
}