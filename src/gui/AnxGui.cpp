#include "gui/AnxGui.hpp"
#include "imgui.h"

AnxGui::AnxGui() {}
AnxGui::~AnxGui() {}

void AnxGui::Draw() {

    if (ImGui::BeginMainMenuBar()) {
        ImGui::TextColored(ImVec4(0.24f, 0.52f, 0.95f, 1.00f), " ANX │");

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open File...", "Ctrl+O")) {  }
            if (ImGui::MenuItem("Save Session", "Ctrl+S")) {  }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {  }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Analysis")) {
            if (ImGui::MenuItem("Auto Analyze", "F5")) { }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About ANX")) {  }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    ImGuiID dockspace_id = ImGui::GetID("My Dockspace");
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

    Console();
    Project();
    Disassembly();
    Properties();
}

void AnxGui::Console() {
    ImGui::Begin("Console");

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), ImGuiChildFlags_None);
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.3f, 1.00f), "[+] Loaded file: /home/monk/Desktop/anx/libanx.so");
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.3f, 1.00f), "[+] Architecture: x86_64");
    ImGui::Text("[*] Parsing symbols table...");
    ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.2f, 1.00f), "[!] Warning: Stripped binary, no debug symbols found.");
    ImGui::EndChild();

    ImGui::Separator();

    static char command_input[256] = "";
    ImGui::Text("anx>"); ImGui::SameLine();
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##ConsoleInput", command_input, sizeof(command_input), ImGuiInputTextFlags_EnterReturnsTrue)) {
        command_input[0] = '\0'; 
    }

    ImGui::End();
}

void AnxGui::Project() {
    ImGui::Begin("Project");

    if (ImGui::CollapsingHeader("Binary Sections", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::BeginTable("SectionsTable", 3, ImGuiTableFlags_BordersInnerV)) {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Address");
            ImGui::TableSetupColumn("Size");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text(".text");
            ImGui::TableNextColumn(); ImGui::Text("0x00401000");
            ImGui::TableNextColumn(); ImGui::Text("0x2500");

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text(".rodata");
            ImGui::TableNextColumn(); ImGui::Text("0x00403500");
            ImGui::TableNextColumn(); ImGui::Text("0x0A00");

            ImGui::EndTable();
        }
    }

    if (ImGui::CollapsingHeader("Functions", ImGuiTreeNodeFlags_DefaultOpen)) {
        const char* functions[] = { "main", "analyze_binary", "parse_elf", "cleanup" };
        static int selected_func = 0;
        for (int i = 0; i < 4; i++) {
            if (ImGui::Selectable(functions[i], selected_func == i)) {
                selected_func = i;
            }
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35.0f);
    ImGui::Separator();

    static char search_buf[256] = "";
    ImGui::InputTextWithHint("##Search", "Search symbols...", search_buf, sizeof(search_buf));

    ImGui::End();
}

void AnxGui::Disassembly() {
    ImGui::Begin("Workspace");

    if (ImGui::BeginTabBar("WorkspaceTabs")) {
        if (ImGui::BeginTabItem("Disassembly")) {

            if (ImGui::BeginTable("DisasmTable", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV)) {
                ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                ImGui::TableSetupColumn("Bytes", ImGuiTableColumnFlags_WidthFixed, 110.0f);
                ImGui::TableSetupColumn("Instruction", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Comment", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0.3f, 0.6f, 0.9f, 1.0f), "0x00401260");
                ImGui::TableNextColumn(); ImGui::Text("55");
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.2f, 1.0f), "push"); ImGui::SameLine(); ImGui::Text("rbp");
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.4f, 1.0f), "; prologue");

                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0.3f, 0.6f, 0.9f, 1.0f), "0x00401261");
                ImGui::TableNextColumn(); ImGui::Text("48 89 e5");
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0.9f, 0.5f, 0.2f, 1.0f), "mov"); ImGui::SameLine(); ImGui::Text("rbp, rsp");
                ImGui::TableNextColumn(); ImGui::Text("");

                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Hex View")) {
            ImGui::Text("00401260: 55 48 89 E5 48 83 EC 30  U H . . H . . 0");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void AnxGui::Properties() {
    ImGui::Begin("Properties");

    ImGui::TextColored(ImVec4(0.24f, 0.52f, 0.95f, 1.00f), "File Properties:");
    ImGui::Separator();
    ImGui::LabelText("Format", "ELF64");
    ImGui::LabelText("Machine", "Advanced Micro Devices X86-64");
    ImGui::LabelText("Entry Point", "0x00401260");

    ImGui::Spacing(); ImGui::Spacing();

    ImGui::TextColored(ImVec4(0.24f, 0.52f, 0.95f, 1.00f), "Processor State (Simulation):");
    ImGui::Separator();
    ImGui::Text("RAX: 0x0000000000000000");
    ImGui::Text("RBP: 0x00007ffeefbfc610");
    ImGui::Text("RSP: 0x00007ffeefbfc5e0");
    ImGui::Text("RIP: 0x0000000000401260");

    ImGui::End();
}
