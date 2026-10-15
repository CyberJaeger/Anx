#pragma once

#include <imgui.h>

class AnxGui {
public:
    AnxGui();
    ~AnxGui();

    void Draw();


private:

    void Console();
    void Project();
    void Disassembly();
    void Properties();

    void ApplyAnxTheme();

    char m_SearchBuffer[128] = "";
    char m_ConsoleBuffer[256] = "";

    int m_SelectedFunctionIndex = 0;
};